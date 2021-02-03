//
// Created by Lukas on 19.01.2021.
//

#include "bitboard.h"

// typedef uint64_t bboard;

// Accessing a square of the bitboard
long get(long b, int square) { return (b & (1ULL << square)); }

long set(long b, int square) { return (b | (1ULL << square)); }

void print_board(long b) {
  int i, j, x;
  char *coord = "ABCDEFGH";
  printf(" ");
  for (x = 0; x < 8; x++) {
    printf(" %c", *(coord + x));
  }
  printf("\n ┌");
  for (x = 0; x < 15; x++) {
    x % 2 == 0 ? printf("─") : printf("┬");
  }
  printf("┐\n");
  for (i = 7; i >= 0; i--) {
    printf("%d│", i + 1);
    for (j = 0; j < 8; j++) {
      printf("%d│", get(b, j + 8 * i) ? 1 : 0);
    }
    printf("%d\n", i + 1);
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
  for (x = 0; x < 8; x++) {
    printf(" %c", *(coord + x));
  }
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
  // Init Startboard
  board->b[0].board = BLACK_STARTING_BOARD;
  board->b[0].depth = 0;
  board->w[0].board = WHITE_STARTING_BOARD;
  board->w[0].depth = 0;
  // Init start Queen's board to empty but type 1 fo Queen
  board->b[1].board = 0ULL;
  board->b[1].depth = 1;
  board->b[1].type = 1;
  board->w[1].board = 0ULL;
  board->w[1].depth = 1;
  board->w[1].type = 1;
  return board;
}

int parsCoordinateToSquare(char *coord) {
  // int row = *(coord) - 'A'; // char coord to int index
  // int column = *(coord+1) - '1'; // char number to int
  // printf("Current Piece: %s, calc coords: row: %d, column: %d\n", coord, row,
  // column);
  return (*(coord) - 'A') + 8 * (*(coord + 1) - '1');
}

int addToBitboardPart(struct bitboardPart *board, int *square, char *type,
                      int *i) {
  board->board = set(board->board, *square);
  board->depth = *i;
  board->type = *type;
  return 0;
}

int addToBitboard(bitboardPart_t bitboards[24], int *square, char *type) {
  if (bitboards == NULL) {
    printf("Error bitboard strut is null\n");
    return -1;
  }
  int i;
  for (i = 0; i < BITBOARDS; i++) {
    if (!get(bitboards[i].board, *square)) {
      if (bitboards[i].type == *type || bitboards[i].type == 0) {
        addToBitboardPart(&bitboards[i], square, type, &i);
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
  char *piece = malloc(sizeof(char) * SIZE_OF_PIECE - 3);
  for (i = 0; i < NUMBER_OF_PIECES; i++) {
    memcpy(piece, piece_list + 2, sizeof(char) * SIZE_OF_PIECE - 3);
    int square = parsCoordinateToSquare(piece + 2);
    switch (*piece) {
    case 'b':
      addToBitboard(board->b, &square, &normal_piece);
      break;
    case 'B':
      addToBitboard(board->b, &square, &king_piece);
      break;
    case 'w':
      addToBitboard(board->w, &square, &normal_piece);
      break;
    case 'W':
      addToBitboard(board->w, &square, &king_piece);
      break;
    default:
      printf("Error color and type <%c> does not exist\n", *piece);
    }
    piece_list += SIZE_OF_PIECE;
  }

  free(piece);

  return board;
}

/*int main() {
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
  bitboard_t *board;// = initBitboard();
  //printBitboard(board);
  board = parsFromString(pieceList);
  printBitboard(board);
}*/