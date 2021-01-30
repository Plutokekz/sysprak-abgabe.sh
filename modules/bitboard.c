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
  /*board->b[0].board = BLACK_STARTING_BOARD;
  board->b[0].depth = 0;
  board->w[0].board = WHITE_STARTING_BOARD;
  board->w[0].depth = 0;*/
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

board_t shift(board_t piece, DIRECTION d) {
  switch (d) {
  case NW:
    return (piece << (COLUMNS + 1)) & ALLOWED_SQUARES;
  case NE:
    return (piece << (COLUMNS - 1)) & ALLOWED_SQUARES;
  case SW:
    return (piece >> (COLUMNS + 1)) & ALLOWED_SQUARES;
  case SE:
    return (piece >> (COLUMNS - 1)) & ALLOWED_SQUARES;
  default:
    printf("wrong direction\n");
    break;
  }
}

board_t captureMoves(board_t playerBoard, board_t opponentBoard,
                     board_t emptyBoard, board_t piece) {
  board_t moves;
  board_t move;
  board_t tmp;
  for (size_t d = NW; d <= SW; d++) {
    move = shift((shift(piece, d) & opponentBoard), d) & emptyBoard;
    moves |= move;
    if (move &&
        (tmp = captureMoves(playerBoard, opponentBoard, emptyBoard, move)))
      moves |= tmp;
  }
  return moves;
}

board_t calculateMoves(bitboard_t *currentBoard, board_t playerBoard,
                       board_t piece, char type, char color) {
  board_t opponentBoard =
      (currentBoard->b[0].board | currentBoard->w[0].board) ^ playerBoard;
  board_t emptyBoard = ~(playerBoard | opponentBoard);
  board_t moves = 0ULL;
  if (type == 'n') {
    // does not support conversion to king after capture move
    moves = captureMoves(playerBoard, opponentBoard, emptyBoard, piece);
    if (moves == 0ULL) {
      if (color == 'w') {
        moves = (shift(piece, NW) | shift(piece, NE)) & emptyBoard;
      } else if (color == 'b') {
        moves = (shift(piece, SW) | shift(piece, SE)) & emptyBoard;
      }
    }
  } else if (type == 'k') {
    board_t move = piece;
    board_t nonCaptureMoves;
    board_t tmp;
    for (size_t d = NW; d <= SW; d++) {
      while ((tmp = shift(move, d)) & emptyBoard) {
        move = tmp;
        nonCaptureMoves |= move;
      }
      moves = captureMoves(playerBoard, opponentBoard, emptyBoard, move);
    }
    moves = moves ? moves : nonCaptureMoves;
  }
  return moves;
}

moveboard_t **allPossibleMoves(bitboard_t *currentBoard, char color) {
  static moveboard_t **movesList;
  if (movesList == NULL) {
    movesList = malloc(12 * sizeof(moveboard_t *));
    for (size_t i = 0; i < 12; i++) {
      movesList[i] = malloc(sizeof(moveboard_t));
    }
  }
  bitboardPart_t parts[BITBOARDS] =
      color == 'w' ? currentBoard->w : currentBoard->b;
  board_t piece;
  int pieceCount = 0;
  char type;
  for (size_t i = 1; i < 64; i++) {
    if ((piece = get(parts[0].board, i))) {
      type = get(parts[1].board, i) ? parts[1].type : parts[0].type;
      board_t moves =
          calculateMoves(currentBoard, parts[0].board, piece, type, color);
      if (moves) {
        movesList[pieceCount]->pieceBoard = piece;
        movesList[pieceCount]->movesBoard = moves;
        pieceCount++;
      }
    }
  }
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