//
// Created by Lukas on 19.01.2021.
//

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

// typedef uint64_t bboard;

// Accessing a square of the bitboard
long get(long b, int square) { return (b & (1ULL << square)); }

long set(long b, int square) { return (b | (1ULL << square)); }

void print_board(long b) {
  int i, j, x;
  char *coord = "ABCDEFGH";
  printf(" ");
  for(x=0;x<8;x++){printf(" %c", *(coord+x));}
  printf("\n ┌");
  for (x = 0; x < 15; x++) {
    x % 2 == 0 ? printf("─") : printf("┬");
  }
  printf("┐\n");
  for (i = 7; i >= 0; i--) {
    printf("%d│", i+1);
    for (j = 0; j < 8; j++) {
      printf("%d│", get(b, j + 8 * i) ? 1 : 0);
    }
    printf("%d\n", i+1);
    if (i >= 1) {
      printf(" ├");
      for (x = 0; x < 15; x++) {
        x % 2 == 0 ? printf("─") : printf("┼");
      }
      printf("┤\n");
    }
  }
  printf(" └");
  for (x = 0; x < 15; x++) {
    x % 2 == 0 ? printf("─") : printf("┴");
  }
  printf("┘\n ");
  for(x=0;x<8;x++){printf(" %c", *(coord+x));}
  printf("\n");
}

void printBitboard(bitboard_t *board) {
  int i;
  long b = 0ULL;
  for (i = 0; i < BITBOARDS; i++) {
    b |= board->w[i].board | board->b[i].board;
  }
  print_board(b);
}

bitboard_t *initBitboard() {

  bitboard_t *board = malloc(sizeof(bitboard_t));

  int i;
  for (i = 0; i < BITBOARDS; i++) {
    board->b[i].color = 'b';
    board->b[i].depth = -1;
    board->b[i].type = 0;
    board->b[i].board = 0ULL;
    board->w[i].color = 'w';
    board->w[i].depth = -1;
    board->w[i].type = 0;
    board->w[i].board = 0ULL;
  }
  /*board->b[0].board = BLACK_STARTING_BOARD;
  board->b[0].depth = 0;
  board->w[0].board = WHITE_STARTING_BOARD;
  board->w[0].depth = 0;*/
  return board;
}

/*
+ PIECESLIST 24
+ b@H8
+ b@F8
+ b@D8
+ b@B8
+ b@G7
+ b@E7
+ b@C7
+ b@A7
+ b@H6
+ b@F6
+ b@D6
+ b@B6
+ w@G3
+ w@E3
+ w@C3
+ w@A3
+ w@H2
+ w@F2
+ w@D2
+ w@B2
+ w@G1
+ w@E1
+ w@C1
+ w@A1
+ ENDPIECESLIST
 */

int parsCoordinateToSquare(char *coord) {
  int row = *(coord) - 'A'; // char coord to int index
  int column = *(coord+1) - '1'; // char number to int
  printf("Current Piece: %s, calc coords: row: %d, column: %d\n", coord, row, column);
  return (*(coord) - 'A') + 8 * (*(coord + 1) - '1');
}
int addWhiteToBitboard(bitboard_t *board, int *square, char *type) {
  if (board == NULL) {
    printf("Error bitboard strut is null\n");
    return -1;
  }
  int i;
  for (i = 0; i < BITBOARDS; i++) {
    //printf("exists: %ld\n", get(board->b[i].board, *square));
    if (!get(board->w[i].board, *square)) {
      //printf("same type: %c == %c ? %d\n", board->w[i].type, *type, board->w[i].type == *type);
      if (board->w[i].type == *type) {
        board->w[i].board = set(board->w[i].board, *square);
        board->w[i].depth = i;
        return 0;

      }
      else if (board->w[i].type == 0) {
        //printf("null type: %c == %c ? %d\n", board->w[i].type, 0, board->w[i].type == 0);
        board->w[i].board = set(board->w[i].board, *square);
        board->w[i].depth = i;
        board->w[i].type = *type;
        return 0;
      }
    }
  }
  printf("Error Out of Bitboards\n");
  return -1;
}

int addBlackToBitboard(bitboard_t *board, int *square, char *type) {
  if (board == NULL) {
    printf("Error bitboard strut is null\n");
    return -1;
  }
  int i;
  for (i = 0; i < BITBOARDS; i++) {
    printf("%ld\n", get(board->b[i].board, *square));
    if (!get(board->b[i].board, *square)) {
      if (board->b[i].type == *type) {
        board->b[i].board = set(board->b[i].board, *square);
        board->b[i].depth = i;
        return 0;
      } else if (board->b[i].type == 0) {
        board->b[i].board = set(board->b[i].board, *square);
        board->b[i].depth = i;
        board->b[i].type = *type;
        return 0;
      }
    }
  }
  printf("Error Out of Bitboards\n");
  return -1;
}

bitboard_t *parsFromString(char *piece_list) {
  bitboard_t *board = initBitboard();
  piece_list += 16; // skipp + PIECESLIST 24\n
  int i;
  char normal_piece = 'n', king_piece = 'k';
  for (i = 0; i < NUMBER_OF_PIECES; i++) {
    char *piece = malloc(sizeof(char) * SIZE_OF_PIECE - 3);
    memcpy(piece, piece_list + 2, sizeof(char) * SIZE_OF_PIECE - 3);

    //printf("square: %d\n", parsCoordinateToSquare(piece + 2));
    int square = parsCoordinateToSquare(piece + 2);

    if (*piece == 'b') {
      //normal black piece
      //printf("|%s|\n", piece);
      addBlackToBitboard(board, &square, &normal_piece);
      printBitboard(board);
    } else if (*piece == 'B') {
      // black king piece
      //printf("|%s|\n", piece);
      addBlackToBitboard(board, &square, &king_piece);
      printBitboard(board);
    } else if (*piece == 'w') {
      // normal white piece
      //printf("|%s|\n", piece);
      addWhiteToBitboard(board, &square, &normal_piece);
      printBitboard(board);
    } else if (*piece == 'W') {
      // white king
      //printf("|%s|\n", piece);
      addWhiteToBitboard(board, &square, &king_piece);
      printBitboard(board);
    } else {
      printf("Error piece <%c> does not exist\n", *piece);
      return NULL;
      // all other pieces make no sense error
    }

    piece_list += SIZE_OF_PIECE;
  }

  // TODO Parse Piece list

  return board;
}

int main() {
  // int row, column;
  // row = 7, column = 7;
  // long b = 0ULL;
  // b = set(b, (row * 8 + column));
  /*b = set(b, (7 * 8 + 1));
  b = set(b, (7 * 8 + 3));
  b = set(b, (7 * 8 + 5));
  b = set(b, (7 * 8 + 7));
  b = set(b, (6 * 8 + 0));
  b = set(b, (6 * 8 + 2));
  b = set(b, (6 * 8 + 4));
  b = set(b, (6 * 8 + 6));
  b = set(b, (5 * 8 + 1));
  b = set(b, (5 * 8 + 3));
  b = set(b, (5 * 8 + 5));
  b = set(b, (5 * 8 + 7));*/

  /*b = set(b, (2 * 8 + 0));
  b = set(b, (2 * 8 + 2));
  b = set(b, (2 * 8 + 4));
  b = set(b, (2 * 8 + 6));
  b = set(b, (1 * 8 + 1));
  b = set(b, (1 * 8 + 3));
  b = set(b, (1 * 8 + 5));
  b = set(b, (1 * 8 + 7));
  b = set(b, (0 * 8 + 0));
  b = set(b, (0 * 8 + 2));
  b = set(b, (0 * 8 + 4));
  b = set(b, (0 * 8 + 6));*/
  long b = BLACK_STARTING_BOARD;
  long w = WHITE_STARTING_BOARD;
  print_board(b);
  print_board(w);
  print_board(b | w);
  printf("%ld\n", b);


  char pieceList[] = "+ PIECESLIST 24\n"
                     "+ b@H8\n"
                     "+ b@F8\n"
                     "+ b@D8\n"
                     "+ b@B8\n"
                     "+ b@G7\n"
                     "+ b@E7\n"
                     "+ b@C7\n"
                     "+ b@A7\n"
                     "+ b@H6\n"
                     "+ b@F6\n"
                     "+ b@D6\n"
                     "+ b@B6\n"
                     "+ w@G3\n"
                     "+ w@E3\n"
                     "+ w@C3\n"
                     "+ w@A3\n"
                     "+ w@H2\n"
                     "+ w@F2\n"
                     "+ w@D2\n"
                     "+ w@B2\n"
                     "+ w@G1\n"
                     "+ w@E1\n"
                     "+ w@C1\n"
                     "+ w@A1\n"
                     "+ ENDPIECESLIST";
  bitboard_t *board = initBitboard();
  printBitboard(board);
  board = parsFromString(pieceList);
  printBitboard(board);
}