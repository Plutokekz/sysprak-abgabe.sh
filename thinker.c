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
    log_debug("thinker guard: %d", ptrGameStart->thinkerGuard);
    if (ptrGameStart->thinkerGuard == 1) {
      ptrGameStart->thinkerGuard = 0;
    } else {
      perror("thinkerGuard not equal 1");
      exit(EXIT_FAILURE);
    }
    memcpy(pieceList, shm + sizeof(struct Share), 300);
    log_debug("Thinker Piece List: %s", pieceList);
    bitboard_t *currentBoard = parsFromString(pieceList);
    if (get_quiet() == false && get_level() >=2){
      printBitboard(currentBoard);
    }
    char playerColor = ptrGameStart->ownPlayerNumber ? 'b' : 'w';
    moveboard_t **moveBoardList = allPossibleMoves(currentBoard, playerColor);
    free(currentBoard);
    freeTowerBoard();
    char moveString[35] = {0};
    pickFirstMove(moveBoardList, moveString);
    log_debug("send: %s", moveString);
    sendCMD(fdw, moveString);
  }
}
