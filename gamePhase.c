#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "performConnection.h"
#include "signal.h"

void gamePhase() {
  char *buffer;

    // Comparing, if the first 10 characters (thereby excluding the newline and nullbyte char respectively) are equal
  while (strncmp(buffer, "+ GAMEOVER", 10) != 0) {
    buffer = recvCommand();

    if (strncmp(buffer, "+ WAIT", 6) == 0){
        sendCommand(OKWAIT, "");
    }

    if (strncmp(buffer, "+ MOVE", 6) == 0){
        sendCommand(THINKING, "");
        // TODO Heinrich: add writing of current gameboard state into SHM

        // TODO Tim : Add sending signal to thinker and print current gameboard
        kill(getppid(), SIGUSR1);
        // TODO Lukas: add communicating move from thinker to connector and writing it into move

        char *move;

        sendCommand (PLAY, move);
    }

    if (strncmp(buffer, "+ QUIT", 6) == 0){
        printf("Server closed connection during gamephase due to protocoll error. \n");
        exit(EXIT_FAILURE);
    }
  }

  //TODO Heinrich: receiving and writing last gamestate into SHM

  //TODO Tim: Printing out last gamestate and who won

}