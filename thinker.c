#include "thinker.h"

void *shm;
int shmId, fdr, fdw, childInit = 0;
char *pieceList;
struct Share *ptrGameStart;

void initThinker(int fd[2]) {
  fdw = fd[1];
  fdr = fd[0];
}

void initChild() {
  pieceList = malloc(300);
  receiveInt(fdr, &shmId);
  shm = attachSHM(shmId);
}

void thinker() {
  if (childInit == 0) {
    initChild();
    childInit = 1;
  } else {

    ptrGameStart = (struct Share *)shm;
    // thinker Guard auswerten
    printf("thinker guard: %d\n", ptrGameStart->thinkerGuard);
    if (ptrGameStart->thinkerGuard == 1) {
      ptrGameStart->thinkerGuard = 0;
    } else {
      perror("thinkerGuard not equal 1");
      exit(EXIT_FAILURE);
    }
    memcpy(pieceList, shm + sizeof(struct Share), 300);
    printf("Thinker Piece List: %s", pieceList);
    bitboard_t *currentBoard = parsFromString(pieceList);
    printBitboard(currentBoard);
    // TODO get player black or white ??
    sendCMD(fdw, "G3:F4"); // A3:B4 B6:A5
  }
}
