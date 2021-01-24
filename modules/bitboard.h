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

bitboard_t *parsFromString(char *piece_list);

void printBitboard(bitboard_t *board);
#endif // BASCHNI_BITBOARD_H
