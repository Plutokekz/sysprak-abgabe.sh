#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "performConnection.h"
#include "signal.h"
#include "shareMemory.h"
#include "cmdPipe.h"

void gamePhase(int fd[2]) {
  char *buffer;
  int shmID;
  void *shmPtr;



  shmID = createSHM(1024);

  sendInt(fd[1], &shmID);

  close(fd[1]);

  shmPtr = attachSHM(shmID);
  int size = 0;

    // Comparing, if the first 10 characters (thereby excluding the newline and nullbyte char respectively) are equal
  while (strncmp(buffer, "+ GAMEOVER", 10) != 0) {
    buffer = recvCommand(1, &size);

    if (strncmp(buffer, "+ WAIT", 6) == 0) {
      free(buffer);
      sendCommand(OKWAIT, "");
    }

    if (strncmp(buffer, "+ MOVE", 6) == 0) {
      sendCommand(THINKING, "");
      printf("move command buffer\n%s", buffer);
      free(buffer);
      buffer = recvCommand(0, &size);

      printf("start piece list\n%s", buffer);

      printf("SHM id in gamePhase %d\n", shmID);

      memcpy(shmPtr, buffer, size);
      //printf("%d\n", size);

        // TODO Tim : Add sending signal to thinker and print current gameboard
        kill(getppid(), SIGUSR1);
        // TODO Lukas: add communicating move from thinker to connector and writing it into move

        char *move;

        if(receiveCMD(*fd[0], move)<0){
          printf("Error receiving cmd\n");
          //Error handling exiting / recalculating the move ?
        }

        sendCommand (PLAY, move);
        free(buffer);
    }

    if (strncmp(buffer, "+ QUIT", 6) == 0) {
      free(buffer);
      printf("Server closed connection during gamephase due to protocoll "
             "error. \n");
      exit(EXIT_FAILURE);
    }
  }
  free (buffer);
  
  strcpy(shmPtr, buffer);

  //TODO Tim: Printing out last gamestate and who won

}