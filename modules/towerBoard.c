#include "towerBoard.h"

#include <stdlib.h>
#include <string.h>

char getType(tower_t *tower) {
  if (tower->height > 0)
    return tower->queue.top->type;
  return 0;
}

char getColor(tower_t *tower) {
  if (tower->height > 0)
    return tower->queue.top->color;
  return 0;
}

// void toKing(tower_t *tower) { tower->queue.top->type = 'k'; }

void initTowerBoard(tower_t **towerBoard) {
  static tower_t *towerList = NULL;
  if (towerList == NULL) {
    towerList = calloc(64, sizeof(tower_t));
  } else {
    memset(towerList, 0, 64 * sizeof(tower_t));
  }
  for (size_t i = 0; i < 64; i++) {
    towerBoard[i] = towerList + i;
  }
}

void freeQueue(queue_t *queue) {
  if (queue->top != NULL) {
    piece_t *piece = dequeue(queue);
    free(piece);
    freeQueue(queue);
  }
}

void enqueue(queue_t *queue, piece_t *piece) {
  if (queue->top == NULL) {
    queue->top = piece;
    queue->bottom = queue->top;
  } else {
    queue->bottom->next = piece;
    queue->bottom = piece;
  }
}

piece_t *dequeue(queue_t *queue) {
  piece_t *piece = queue->top;
  queue->top = queue->top->next;
  return piece;
}

char capture(tower_t **old, tower_t **new, tower_t *captured) {
  piece_t *capturedPiece = dequeue(&captured->queue);
  // swap towers
  tower_t *tmp = *new;
  *new = *old;
  *old = tmp;
  enqueue(&(*new)->queue, capturedPiece);
  captured->height--;
  (*new)->height++;
  return getColor(captured);
}

void moveTower(tower_t **old, tower_t **new) {
  tower_t *tmp = *new;
  *new = *old;
  *old = tmp;
}

void pushPiece(tower_t *tower, piece_t *piece) {
  if (tower->queue.top == NULL)
    tower->queue.bottom = piece;
  piece->next = tower->queue.top;
  tower->queue.top = piece;
  (tower->height)++;
}