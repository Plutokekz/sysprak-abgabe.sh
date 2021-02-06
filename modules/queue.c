#include "queue.h"


char getType(tower_t *tower) {
    return tower->queue->first->type;
}

char getColor(tower_t *tower){
    return tower->queue->first->color;
}

void toKing(tower_t *tower){
    tower->queue->first->type = 'k';
}

tower_t *initTower(){

}

void capture(tower_t *tower, tower_t *target){

}