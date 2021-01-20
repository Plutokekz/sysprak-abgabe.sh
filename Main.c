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
#include <signal.h>
#include <pthread.h>

#include "gamePhase.h"
#include "modules/config.h"
#include "modules/shareMemory.h"
#include "performConnection.h"
#include "thinker.h"

int fd[2];



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
void setOptions(int argc, char *argv[], opt_t *opt, config_t **config, P_FLAG *f) {
  int c;
  while ((c = getopt(argc, argv, "g:p:c:d")) != -1) {
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
    case 'd':
      *f = DEBUG;
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

void handleSigusr1 () {
  write(0, "Ahhh! sig works!\n", 17);
  thinker(fd);
}

/** @brief set Options and splits up program in Thinker and Connector
 *
 */
int main(int argc, char *argv[]) {
  pid_t pid;
  opt_t opt = {"", ""};
  config_t *config = NULL;
  P_FLAG f = PRETTY;
  //int fd[2];

  setOptions(argc, argv, &opt, &config, &f);

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
    printf("child pid: %d\n", getpid());
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      freeConfig(config);
      perror("creating socket failed");
      return EXIT_FAILURE;
    }
    int shmID = performConnection(sock, &opt, config, f);
    printf("shmID in Child proc: %d\n", shmID);

      //Pipe für shmID
    //close(fd[0]); //entweder var in perfC oder shmID ausgeben aus perfC
    if (write(fd[1], &shmID, sizeof(int)) < 0) {
      perror("Error writing to pipe");
    }
    gamePhase(fd);

    //Signal--------------
    kill(getppid(), SIGUSR1);
    //---------------------
    close(sock);
  } else {
    // parent process thinker
    freeConfig(config);

    struct sigaction sa = { 0 };
    sa.sa_handler = &handleSigusr1;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, NULL);


    printf("Pid in main.c before waitpid: %d\n", pid);
    // last part of thinker; executed after game over
    if (wait(NULL) == -1) {
      freeConfig(config);
      perror("error waiting for connector");
      return EXIT_FAILURE;
    }
    

  }

  return EXIT_SUCCESS;
}
