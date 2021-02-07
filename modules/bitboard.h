//
// Created by Lukas on 21.01.2021.
//

#ifndef BASCHNI_BITBOARD_H
#define BASCHNI_BITBOARD_H
#include <stdbool.h>
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
typedef enum PIECE_TYPE { NORMAL, QUEEN, NOT_IN_USE } PIECE_TYPE;
typedef struct bitboardPart {
  unsigned long board;
  char color;
  PIECE_TYPE type;
  char depth;
} bitboardPart_t;

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
