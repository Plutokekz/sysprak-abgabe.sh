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
#include <fcntl.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "gamePhase.h"
#include "modules/shareMemory.h"
#include "modules/utils.h"
#include "performConnection.h"
#include "thinker.h"

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

void thinking(){
  thinker();
}

void child(config_t *config, int fd[2]) {
  int sock;
  // printf("child pid: %d\n", getpid());
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    freeConfig(config);
    perror("creating socket failed");
    exit(EXIT_FAILURE);
  }
  performConnection(sock, config);
  // Setup SHM for gameInfo
  game_info *gameInfo = getGameInfo();
  int shmID = setupSHM_GameStart(gameInfo);
  freeGameInfo(gameInfo);
  // Pipe für shmID
  // close(fd[0]); //entweder var in perfC oder shmID ausgeben aus perfC
  if (write(fd[1], &shmID, sizeof(int)) < 0) {
    perror("Error writing to pipe");
  }
  gamePhase(sock, fd, shmID);

  close(sock);
}

void parent(config_t *config) {
  freeConfig(config);
  struct sigaction sa = {0};
  sa.sa_handler = &thinking;
  sa.sa_flags = SA_RESTART;
  sigaction(SIGUSR1, &sa, NULL);



  // printf("Pid in main.c before waitpid: %d\n", pid);
  // last part of thinker; executed after game over
  if (wait(NULL) == -1) {
    perror("error waiting for connector");
    exit(EXIT_FAILURE);
  }
}

/** @brief set Options and splits up program in Thinker and Connector
 *
 */
int main(int argc, char *argv[]) {
  pid_t pid;
  int fd[2];

  config_t *config = setOptions(argc, argv);

  if (pipe(fd) < 0) {
    perror("Error creating pipe.");
    exit(EXIT_FAILURE);
  }
  init(fd);
  // splitting up in thinker and connector
  if ((pid = fork()) < 0) {
    freeConfig(config);
    perror("fork failed");
    return EXIT_FAILURE;
  } else if (pid == 0) {
    child(config, fd);

  } else {
    parent(config);
  }
  return EXIT_SUCCESS;
}
