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

#include "towerBoard.h"

#define BITBOARDS 24
#define BLACK_STARTING_BOARD -6172840797264674816
#define WHITE_STARTING_BOARD 5614165
#define NUMBER_OF_PIECES 24
#define SIZE_OF_PIECE 7
#define COLUMNS 8
#define ALLOWED_SQUARES 12273903644374837845ULL
#define WHITE_BASELINE 281479271743489ULL
#define BLACK_BASELINE 9223512776490647552ULL

typedef unsigned long long board_t;

typedef enum DIRECTION { NW, NE, SW, SE, UNDEFINED } DIRECTION;

typedef struct bitboard {
  board_t whiteBoard;
  board_t whiteQueenBoard;
  board_t blackBoard;
  board_t blackQueenBoard;
  board_t emptyBoard;
} bitboard_t;

typedef struct MoveBoard {
  board_t pieceBoard;
  board_t movesList[12];
} moveboard_t;

extern char BITBOARD_LOOKUP[64][3];

extern int LONG_BITBOARD_LOOKUP[59];

extern int allowedSquaresIndices[32];

extern tower_t *TOWER_BOARD[64];

void printBitboard(bitboard_t *board);

bitboard_t *parsFromString(char *piece_list);

moveboard_t **allPossibleMoves(bitboard_t *currentBoard, char color);

void pickFirstMove(moveboard_t **moveBoardList, char *moveString);

#endif // BASCHNI_BITBOARD_H
