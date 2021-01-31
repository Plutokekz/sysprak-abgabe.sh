//
// Created by Lukas on 21.01.2021.
//

#ifndef BASCHNI_BITBOARD_H
#define BASCHNI_BITBOARD_H
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct bitboardPart {
  unsigned long board;
  char color;
  char type;
  char depth;
} bitboardPart_t;

typedef struct bitboard {
  bitboardPart_t w[BITBOARDS];
  bitboardPart_t b[BITBOARDS];
} bitboard_t;

typedef struct MoveBoard {
  unsigned long pieceBoard;
  unsigned long movesBoard;
} moveboard_t;

bitboard_t *parsFromString(char *piece_list);

int allowedSquaresIndices[] = {0,  2,  4,  6,  9,  11, 13, 15, 16, 18, 20,
                               22, 25, 27, 29, 31, 32, 34, 36, 38, 41, 43,
                               45, 47, 48, 50, 52, 54, 57, 59, 61, 63};

void printBitboard(bitboard_t *board);
#endif // BASCHNI_BITBOARD_H
