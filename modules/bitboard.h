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
#define ALLOWED_SQUARES 6148914691236517205

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

void printBitboard(bitboard_t *board);
#endif // BASCHNI_BITBOARD_H
