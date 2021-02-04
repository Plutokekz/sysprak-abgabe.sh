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

board_t shift(board_t piece, DIRECTION d) {
  switch (d) {
  case NW:
    return (piece << (COLUMNS - 1)) & ALLOWED_SQUARES;
  case NE:
    return (piece << (COLUMNS + 1)) & ALLOWED_SQUARES;
  case SW:
    return (piece >> (COLUMNS + 1)) & ALLOWED_SQUARES;
  case SE:
    return (piece >> (COLUMNS - 1)) & ALLOWED_SQUARES;
  default:
    printf("wrong direction\n");
    return 0ULL;
  }
}

void captureMoves(board_t playerBoard, board_t opponentBoard,
                  board_t emptyBoard, board_t piece, char type, char color,
                  board_t *movesList, int depth) {
  board_t tmpOpponentBoard = opponentBoard;
  board_t move;
  board_t tmp;
  char currentType = type;
  for (size_t d = NW; d <= SW; d++) {
    move = shift((shift(piece, d) & opponentBoard), d) & emptyBoard;
    if (move) {
      tmpOpponentBoard = opponentBoard & (~shift(move, (d + 1) % 4));
      movesList[depth] |= move;
      if ((color == 'b' && (move & WHITE_BASELINE)) ||
          (color == 'w' && (move & BLACK_BASELINE)))
        currentType = 'k';
      if (move && currentType == 'k') {
        while ((tmp = shift(move, d)) & emptyBoard)
          move = tmp;
      }
      captureMoves(playerBoard, tmpOpponentBoard, emptyBoard, move, currentType,
                   color, movesList, depth + 1);
    }
  }
}

board_t *calculateMoves(board_t playerBoard, board_t opponentBoard,
                        board_t piece, char type, char color) {
  static board_t *movesList;
  if (movesList == NULL) {
    movesList = calloc(12, sizeof(board_t));
  } else {
    for (size_t i = 0; i < 12; i++) {
      movesList[i] = 0ULL;
    }
  }
  board_t emptyBoard = ~(playerBoard | opponentBoard);
  if (type == 'n') {
    captureMoves(playerBoard, opponentBoard, emptyBoard, piece, type, color,
                 movesList, 0);
    if (movesList[0] == 0ULL) {
      if (color == 'w') {
        movesList[0] = (shift(piece, NW) | shift(piece, NE)) & emptyBoard;
      } else if (color == 'b') {
        movesList[0] = (shift(piece, SW) | shift(piece, SE)) & emptyBoard;
      }
    }
  } else if (type == 'k') {
    board_t move = piece;
    board_t nonCaptureMoves = 0ULL;
    board_t tmp;
    for (size_t d = NW; d <= SW; d++) {
      while ((tmp = shift(move, d)) & emptyBoard) {
        move = tmp;
        nonCaptureMoves |= move;
      }
      captureMoves(playerBoard, opponentBoard, emptyBoard, move, type, color,
                   movesList, 0);
    }
    if (movesList[0] == 0ULL)
      movesList[0] = nonCaptureMoves;
  }
  return movesList;
}

moveboard_t **allPossibleMoves(bitboard_t *currentBoard, char color) {
  static moveboard_t **moveBoardList;
  if (moveBoardList == NULL) {
    moveBoardList = malloc(12 * sizeof(moveboard_t *));
    for (size_t i = 0; i < 12; i++) {
      moveBoardList[i] = malloc(sizeof(moveboard_t));
    }
  } else {
    for (size_t i = 0; i < 12; i++) {
      moveBoardList[i]->pieceBoard = 0ULL;
      moveBoardList[i]->movesList = NULL;
    }
  }
  bitboardPart_t *parts = color == 'w' ? currentBoard->w : currentBoard->b;
  board_t playerBoard = parts[0].board;
  board_t opponentBoard =
      (currentBoard->b[0].board | currentBoard->w[0].board) ^ playerBoard;
  board_t piece;
  int count = 0;
  char type;
  for (size_t i = 1; i < 32; i++) {
    if ((piece = get(parts[0].board, allowedSquaresIndices[i]))) {
      type = get(parts[1].board, allowedSquaresIndices[i]) ? parts[1].type
                                                           : parts[0].type;
      board_t *movesList =
          calculateMoves(playerBoard, opponentBoard, piece, type, color);
      if (movesList[0]) {
        moveBoardList[count]->pieceBoard = piece;
        moveBoardList[count]->movesList = movesList;
        count++;
      }
    }
  }
  return moveBoardList;
}

int main() {
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
  bitboard_t *currentBoard = parsFromString(pieceList);
  // printBitboard(currentBoard);
  board_t piece = 512ULL;
  board_t playerBoard = 512ULL;
  board_t opponentBoard = shift(playerBoard, NE);
  opponentBoard |= shift(shift(opponentBoard, NE), NE);
  board_t emptyBoard = ~(playerBoard | opponentBoard);
  board_t *movesList =
      calculateMoves(playerBoard, opponentBoard, piece, 'n', 'w');
  print_board(movesList[1]);
  // moveboard_t **moveBoardList = allPossibleMoves(currentBoard, 'w');
  // int i = 0;
  // while (moveBoardList[i++]->pieceBoard) {
  //  print_board(moveBoardList[i]->pieceBoard);
  //  print_board(moveBoardList[i]->movesBoard);
  //}
}