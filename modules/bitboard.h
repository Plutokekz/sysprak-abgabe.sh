//
// Created by Lukas on 21.01.2021.
//

#ifndef BASCHNI_BITBOARD_H
#define BASCHNI_BITBOARD_H
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BITBOARDS 24
#define BLACK_STARTING_BOARD -6172840797264674816
#define WHITE_STARTING_BOARD 5614165
#define NUMBER_OF_PIECES 24
#define SIZE_OF_PIECE 7
#define COLUMNS 8
#define ALLOWED_SQUARES 12273903644374837845ULL
#define WHITE_BASELINE 255ULL
#define BLACK_BASELINE 18374686479671623680ULL

typedef unsigned long long board_t;

typedef enum DIRECTION { NW, NE, SW, SE } DIRECTION;

// One bitboard layer
typedef struct bitboardPart {
  unsigned long board;
  char color;
  char type;
  char depth;
} bitboardPart_t;

// 24 bitboard layers per colour. Top one is for top stones (ones that can move), second to top is to show where the kings are,
// all layers below are for pieces which have pieces on top of them and therefore can't move.
// Layers 3-13 are for normal pieces, layers 14-24 are for kings which are under other pieces.
typedef struct bitboard {
  bitboardPart_t w[BITBOARDS];
  bitboardPart_t b[BITBOARDS];
} bitboard_t;

typedef struct MoveBoard {
  board_t pieceBoard;
  board_t movesList[12];
} moveboard_t;

bitboard_t *parsFromString(char *piece_list);

void printBitboard(bitboard_t *board);

moveboard_t **allPossibleMoves(bitboard_t *currentBoard, char color);

void pickFirstMove(moveboard_t **moveBoardList, char *moveString);

#endif // BASCHNI_BITBOARD_H
