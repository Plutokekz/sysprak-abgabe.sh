#include "towerBoard.h"

#include <stdlib.h>
#include <string.h>

char getType(tower_t *tower) {
    return tower->queue.top->type;
}

char getColor(tower_t *tower){
    return tower->queue.top->color;
}

void toKing(tower_t *tower){
    tower->queue.top->type = 'k';
}

void initTower(tower_t **towerBoard, int *allowedSquaresIndices){
    static tower_t *towerList = NULL;
    if (NULL) {
        towerList = malloc(32 * sizeof(tower_t));
    } else {
        memset(towerList, 0, 32 * sizeof(tower_t));
    }
    for (size_t i = 0; i < 32; i++) {
        towerBoard[allowedSquaresIndices[i]] = towerList + i;
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

piece_t *capture(tower_t **old, tower_t **new, tower_t *captured){
    piece_t *capturedPiece = dequeue(&captured->queue);
    // swap towers
    tower_t *tmp = *new;
    *new = *old;
    *old = tmp;
    enqueue(&(*new)->queue, capturedPiece);
    return capturedPiece;
}

void pushPiece(tower_t *tower, piece_t *piece) {
    if (tower->queue.top == NULL)
        tower->queue.bottom = piece;
    piece->next = tower->queue.top;
    tower->queue.top = piece;
}