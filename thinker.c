#include "thinker.h"
#include "modules/cmdPipe.h"
#include "modules/shareMemory.h"

void thinker(int *fd) {

  // Thinker*************
  int shmID;
  close(fd[1]);
  if (read(fd[0], &shmID, sizeof(int)) < 0) {
    perror("Error writing to pipe");
  }

  struct Share *ptrGameStart, gameStart;
  ptrGameStart = &gameStart;
  ptrGameStart = attachSHM(shmID);

  // thinker Guard auswerten
  if (ptrGameStart->thinkerGuard == 1) {
    ptrGameStart->thinkerGuard = 0;

    // insert print spielfeld

  } else {
    perror("thinkerGuard not equal 1");
    exit(EXIT_FAILURE);
  }
  int raw_shmId;
  receiveInt(fd[0], &raw_shmId);

  printf("SHM id in thinker %d\n", shmID);

  char *pieceList = malloc(7 * 24);

  void *shm = attachSHM(raw_shmId);

  memcpy(pieceList, shm, 7 * 24);

  printf("%s", pieceList);


  /*printf("shmID in thinker.c: %d\n", shmID);
  printf("in thinker(), Erfolg: %s\n", (*ptrGameStart).gameName);
  printf("numofPlayers: %d\n", (*ptrGameStart).numberOfPlayers);
  printf("thinkerPID, in thinker.c: %d\n", (*ptrGameStart).thinkerPID);
  printf("player 0: %d\n", ptrGameStart->players[0].number);*/
  deleteSHM(shmID);
  deleteSHM(raw_shmId);
}
