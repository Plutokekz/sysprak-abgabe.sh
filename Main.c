/** @file Main.c
 *
 *  @brief Main file of the program.
 *
 *  This file is executed first.
 *  It contains the whole logic for connecting the different modules of the
 * project.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "config.h"
#include "performConnection.h"

/** @brief Parse arguments
 *
 * Parse arguments, copy them to Opt struct and if possible sets the config
 * struct pointer to the created config struct.
 *
 * -g gameId
 * -p playerId
 * -c configFilename
 *
 *
 *  @param argc length of argv
 *  @param argv char array with the args
 *  @param *opt opt_t struct
 *  @param **config pointer to a pointer of a config_t struct
 *
 */
void setOptions(int argc, char *argv[], opt_t *opt, config_t **config) {
  int c;
  while ((c = getopt(argc, argv, "g:p:c:")) != -1) {
    switch (c) {
    case 'g':
      if (strlen(optarg) != GAME_ID_SIZE) {
        printf("wrong game id\n");
        exit(EXIT_FAILURE);
      }
      strcpy(opt->gameId, optarg);
      break;
    case 'p': {
      unsigned int id;
      if (sscanf(optarg, "%ui", &id) < 1 || !(id == 1 || id == 2)) {
        printf("wrong player id\n");
        exit(EXIT_FAILURE);
      }
      strcpy(opt->playerId, optarg);
      break;
    }
    case 'c':
      *config = readConfigFile(optarg);
    break;
    case '?':
      printf("wrong argument\n");
      freeConfig(*config);
      exit(EXIT_FAILURE);
      break;

    default:
      break;
    }
  }

  // temporary add game id in Makefile
  if (strlen(opt->gameId) == 0) {
    printf("game id is missing\n");
    exit(EXIT_FAILURE);
  }
}

/** @brief set Options and splits up program in Thinker and Connector
 *
 */
int main(int argc, char *argv[]) {
  pid_t pid;
  opt_t opt = {"", ""};
  config_t *config = NULL;

  setOptions(argc, argv, &opt, &config);


  // splitting up in thinker and connector
  if ((pid = fork()) < 0) {
    freeConfig(config);
    perror("fork failed");
    return EXIT_FAILURE;
  } else if (pid == 0) {
    // child process connector
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      freeConfig(config);
      perror("creating socket failed");
      return EXIT_FAILURE;
    }
    performConnection(sock, &opt, config);
    close(sock);
  } else {
    // parent process thinker
    freeConfig(config);

    // last part of thinker; executed after game over
    if (waitpid(pid, NULL, 0) == -1) {
      freeConfig(config);
      perror("error waiting for connector");
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
