/** @file Main.c
 * 
 *  @brief Main file of the programm.
 * 
 *  This file is executed first.
 *  It contains the whole logic for connecting the different modules of the project.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "performConnection.h"

/** @brief Parse arguments and copy them to Opt struct.
 * 
 *  @param argc, argv from main function and Opt struct to modify
 * 
 */
void setOptions(int argc, char *argv[], opt_t *opt) {
  int c;
  while ((c = getopt(argc, argv, "g:p:")) != -1) {
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
    case '?':
      printf("wrong argument\n");
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

/** @brief set Options and splits up programm in Thinker and Connector
 * 
 */
int main(int argc, char *argv[]) {
  pid_t pid;
  opt_t opt = {"", ""};
  setOptions(argc, argv, &opt);

  // splitting up in thinker and connector
  if ((pid = fork()) < 0) {
    perror("fork failed");
    return EXIT_FAILURE;
  } else if (pid == 0) {
    // child process connector
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      perror("creating socket failed");
      return EXIT_FAILURE;
    }
    performConnection(sock, &opt);
    close(sock);
  } else {
    // parent process thinker

    //last part of thinker; executed after game over
    if (waitpid(pid, NULL, 0) == -1) {
      perror("error waiting for connector");
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
