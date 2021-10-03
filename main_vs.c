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

#include "game_phase.h"
#include "modules/cmd_pipe.h"
#include "modules/share_memory.h"
#include "modules/utils.h"
#include "perform_connection.h"
#include "thinker.h"
#include "modules/my_signal.h"

void child(config_t *config, int fd[2]) {
  int sock, shmID;
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    freeConfig(config);
    perror("creating socket failed");
    exit(EXIT_FAILURE);
  }
  performConnection(sock, config);
  freeConfig(config);
  // Setup SHM for gameInfo
  game_info *gameInfo = getGameInfo();
  shmID = setupSHM_GameStart(gameInfo);
  freeGameInfo(gameInfo);

  sendInt(fd[1], &shmID);   // send shmid to thinker
  kill(getppid(), SIGUSR1); // init thinker
  gamePhase(sock, fd, shmID);

  close(sock);
}

void parent(config_t *config) {
  freeConfig(config);
  activateSignal();

  if (wait(NULL) == -1) {
    perror("error waiting for connector");
    exit(EXIT_FAILURE);
  }
}

/** @brief set Options and splits up program in Thinker and Connector
 *
 */
int run_client(int argc, char *argv[]) {
  pid_t pid;
  int fd[2];

  config_t *config = setOptions(argc, argv);

  if (pipe(fd) < 0) {
    perror("Error creating pipe.");
    exit(EXIT_FAILURE);
  }
  initThinker(fd);
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

int main(int argc, char *argv[]) {
  pid_t pid;
    if ((pid = fork()) < 0){
        perror("fork faild");
        return EXIT_FAILURE;
    } else if (pid == 0) {
        // child
        printf("Child\n");
        run_client(argc, argv);
    } else{
        // parent
        printf("Parent\n");
        run_client(argc, argv);
        if (wait(NULL) == -1) {
            perror("error waiting for other client");
            exit(EXIT_FAILURE);
        }
    }
    return EXIT_SUCCESS;
  }
