/** @file Main.c
 *
 *  @brief Main file of the program.
 *
 *  This file is executed first.
 *  It contains the whole logic for connecting the different modules of the
 * project.
 */

/*! \mainpage My Personal Index Page
 *
 * \section intro_sec Introduction
 *
 * This is the introduction.
 *
 * \section install_sec Installation
 *
 * \subsection step1 Step 1: Opening the box
 *
 * \htmlonly
 * <iframe width="560" height="315"
 * src="https://www.youtube.com/embed/KXl7xGMg9qQ?controls=0" frameborder="0"
 * allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope;
 * picture-in-picture" allowfullscreen></iframe> \endhtmlonly
 *
 *
 * etc...
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>

#include "config.h"
#include "performConnection.h"
#include "shareMemory.h" //evtl. unnötig

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
      if (sscanf(optarg, "%u", &id) < 1 || id < 1 || id > 2) {
        printf("wrong player id\n");
        exit(EXIT_FAILURE);
      }
      snprintf(opt->playerId, 2, "%u", id - 1);
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
  int fd[2];

  setOptions(argc, argv, &opt, &config);

  if (pipe(fd) < 0) {
    perror("Error creating pipe.");
    exit(EXIT_FAILURE);
  }

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

    //Thinker
    

    // last part of thinker; executed after game over
    if (waitpid(pid, NULL, 0) == -1) {
      freeConfig(config);
      perror("error waiting for connector");
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
