#ifndef BASCHNI_QUEUE_H
#define BASCHNI_QUEUE_H

#include <stddef.h>
#include "log.h"

// piece in queue
// colour may be 'b' or 'w'
// type may be 'n' or 'k'
typedef struct piece {
    char color;
    char type;
    struct piece *next;
} piece_t;

// first is the piece on top (the one getting dequeued next), last the one on the bottom
typedef struct queue {
    piece_t *top;
    piece_t *bottom;
} queue_t;

typedef struct tower {
    size_t height;
    queue_t queue;
} tower_t;

void initTowerBoard(tower_t **towerBoard);

void freeQueue(queue_t *queue);

char getType(tower_t *tower);

char getColor(tower_t *tower);

void toKing(tower_t *tower);

char capture(tower_t **old, tower_t **new, tower_t *captured);

void moveTower(tower_t **old, tower_t **new);

// Used when building the towers by parsing serverinput. Being built as stacks, pushPiece pushes a new piece onto the stack.
void pushPiece(tower_t *tower, piece_t *piece);

void enqueue(queue_t *queue, piece_t *piece);

piece_t *dequeue(queue_t *queue);


#endif // BASCHNI_QUEUE_H
