//
// Created by Lukas on 19.01.2021.
//

#include "bitboard.h"
char *rows = "ABCDEFGH";
char WHITE = 'w', WHITE_QUEEN = 'W', BLACK = 'b', BLACK_QUEEN = 'B';
char *colon = ":";

char BITBOARD_LOOKUP[64][3] = {
    "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "B1", "B2", "B3",
    "B4", "B5", "B6", "B7", "B8", "C1", "C2", "C3", "C4", "C5", "C6",
    "C7", "C8", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "E1",
    "E2", "E3", "E4", "E5", "E6", "E7", "E8", "F1", "F2", "F3", "F4",
    "F5", "F6", "F7", "F8", "G1", "G2", "G3", "G4", "G5", "G6", "G7",
    "G8", "H1", "H2", "H3", "H4", "H5", "H6", "H7", "H8"};

int LONG_BITBOARD_LOOKUP[59] = {-1, 0,  59, 50, 2,  6,  -1, 18, 61, -1, -1, 25,
                                52, 45, -1, -1, 4,  -1, 43, 38, -1, -1, -1, 15,
                                -1, -1, -1, 34, 20, -1, 57, -1, 63, -1, 41, -1,
                                -1, -1, -1, -1, 9,  -1, 11, -1, 27, 48, 16, -1,
                                54, 36, 13, 32, 47, 22, -1, 31, -1, -1, 29};

static int allowedSquaresIndices[32] = {
    0,  2,  4,  6,  9,  11, 13, 15, 16, 18, 20, 22, 25, 27, 29, 31,
    32, 34, 36, 38, 41, 43, 45, 47, 48, 50, 52, 54, 57, 59, 61, 63};

tower_t *TOWER_BOARD[64] = {NULL};

// Accessing a square of the bitboard
long get(long b, int square) { return (b & (1ULL << square)); }

long set(long b, int square) { return (b | (1ULL << square)); }

char *bitBoardToChar(board_t board) {
  return BITBOARD_LOOKUP[LONG_BITBOARD_LOOKUP[board % 59]];
}

tower_t *getTower(board_t board) {
  return TOWER_BOARD[LONG_BITBOARD_LOOKUP[board % 59]];
}

void print_board(char *b) {
  int i, j, x;
  printf(" ");
  for (x = 0; x < 8; x++) {
    printf("  %c ", *(rows + x));
  }
  printf("\n ┌");
  for (x = 0; x < 15; x++) {
    x % 2 == 0 ? printf("───") : printf("┬");
  }
  printf("┐\n");
  for (i = 7; i >= 0; i--) {
    printf("%d│", i + 1);
    for (j = 0; j < 8; j++) {
      printf(" %c │", b[j * 8 + i]);
    }
    printf("%d\n", i + 1);
    if (i >= 1) {
      printf(" ├");
      for (x = 0; x < 15; x++) {
        x % 2 == 0 ? printf("───") : printf("┼");
      }
      printf("┤\n");
    }
  }
  printf(" └");
  for (x = 0; x < 15; x++) {
    x % 2 == 0 ? printf("───") : printf("┴");
  }
  printf("┘\n ");
  for (x = 0; x < 8; x++) {
    printf("  %c ", *(rows + x));
  }
  printf("\n");
}

void bitboardToArray(char *b, long long bitboard, char *color) {
  int i, j;
  for (i = 7; i >= 0; i--) {
    for (j = 0; j < 8; j++) {
      if (get(bitboard, (j * 8 + i))) {
        b[j * 8 + i] = *color;
      }
    }
  }
}

void printBitboard(bitboard_t *board) {
  int i;
  char *b = malloc(sizeof(char) * 64);
  for (i = 0; i < 64; i++) {
    b[i] = ' ';
  }

  bitboardToArray(b, board->w[0].board, &WHITE);
  bitboardToArray(b, board->w[1].board, &WHITE_QUEEN);

  bitboardToArray(b, board->b[0].board, &BLACK);
  bitboardToArray(b, board->b[1].board, &BLACK_QUEEN);

  print_board(b);

  free(b);
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
  board->b[0].board = 0; // BLACK_STARTING_BOARD;
  board->b[0].depth = 0;
  board->w[0].board = 0; // WHITE_STARTING_BOARD;
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
  return (*(coord) - 'A') * 8 + (*(coord + 1) - '1');
}

int addToBitboardPart(struct bitboardPart *board, int *square, char type,
                      int *i) {
  board->board = set(board->board, *square);
  board->depth = *i;
  board->type = type;
  return 0;
}

int addToBitboard(bitboardPart_t bitboards[24], int *square, char type) {
  if (bitboards == NULL) {
    printf("Error bitboard strut is null\n");
    return -1;
  }
  int i;
  for (i = 0; i < BITBOARDS; i++) {
    if (!get(bitboards[i].board, *square)) {
      if (bitboards[i].type == type || bitboards[i].type == 0) {
        addToBitboardPart(&bitboards[i], square, type, &i);
        return 0;
      }
    }
  }
  printf("Error Out of Bitboards\n");
  return -1;
}

/*
bitboard_t *parsFromString(char *piece_list) {
  bitboard_t *board = initBitboard();
  // 24 can change
  piece_list += 16; // skipp + PIECESLIST 24\n
  int i;
  char normal_piece = 'n', king_piece = 'k';
  // why malloc
  char *piece = malloc(sizeof(char) * SIZE_OF_PIECE - 3);
  for (i = 0; i < NUMBER_OF_PIECES; i++) {
    memcpy(piece, piece_list + 2, sizeof(char) * SIZE_OF_PIECE - 3);
    int square = parsCoordinateToSquare(piece + 2);
    switch (*piece) {
    case 'b':
      // why passing pointer instead of int
      addToBitboard(board->b, &square, normal_piece);
      break;
    case 'B':
      addToBitboard(board->b, &square, king_piece);
      break;
    case 'w':
      addToBitboard(board->w, &square, normal_piece);
      break;
    case 'W':
      addToBitboard(board->w, &square, king_piece);
      break;
    default:
      printf("Error color and type <%c> does not exist\n", *piece);
    }
    piece_list += SIZE_OF_PIECE;
  }

  free(piece);

  return board;
}
*/

bitboard_t parsFromString(char *pieceList) {
  initTowerBoard()
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
  board_t tmpEmptyBoard = emptyBoard;
  board_t move;
  board_t tmp;
  char currentType = type;
  for (size_t d = NW; d <= SE; d++) {
    move = shift((shift(piece, d) & opponentBoard), d) & emptyBoard;
    if (move) {
      tmpOpponentBoard = opponentBoard & (~shift(move, (d + 1) % 4));
      tmpEmptyBoard = ~(playerBoard | tmpOpponentBoard);
      movesList[depth] |= move;
      if ((color == 'b' && (move & WHITE_BASELINE)) ||
          (color == 'w' && (move & BLACK_BASELINE)))
        currentType = 'k';
      if (move && currentType == 'k') {
        while ((tmp = shift(move, d)) & tmpEmptyBoard)
          move = tmp;
      }
      captureMoves(playerBoard, tmpOpponentBoard, tmpEmptyBoard, move,
                   currentType, color, movesList, depth + 1);
    }
  }
}

board_t *calculateMoves(board_t playerBoard, board_t opponentBoard,
                        board_t piece, char type, char color,
                        bool *captureMovesOnly) {
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
    if (movesList[0] == 0ULL && !(*captureMovesOnly)) {
      if (color == 'w') {
        movesList[0] = (shift(piece, NW) | shift(piece, NE)) & emptyBoard;
      } else if (color == 'b') {
        movesList[0] = (shift(piece, SW) | shift(piece, SE)) & emptyBoard;
      }
    } else {
      *captureMovesOnly = true;
    }
  } else if (type == 'k') {
    board_t move = piece;
    board_t nonCaptureMoves = 0ULL;
    board_t tmp;
    for (size_t d = NW; d <= SE; d++) {
      while ((tmp = shift(move, d)) & emptyBoard) {
        move = tmp;
        nonCaptureMoves |= move;
      }
      captureMoves(playerBoard, opponentBoard, emptyBoard, move, type, color,
                   movesList, 0);
    }
    if (movesList[0] == 0ULL && !(*captureMovesOnly)) {
      movesList[0] = nonCaptureMoves;
    } else {
      *captureMovesOnly = true;
    }
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
      memset(moveBoardList[i]->movesList, 0, 12 * sizeof(board_t));
    }
  }
  bitboardPart_t *parts = color == 'w' ? currentBoard->w : currentBoard->b;
  board_t playerBoard = parts[0].board;
  board_t opponentBoard =
      (currentBoard->b[0].board | currentBoard->w[0].board) ^ playerBoard;
  board_t piece;
  board_t *movesList;
  int count = 0;
  char type;
  bool captureMovesOnly = false;
  bool tmp;
  for (size_t i = 0; i < 32; i++) {
    if ((piece = get(parts[0].board, allowedSquaresIndices[i]))) {
      type = get(parts[1].board, allowedSquaresIndices[i]) ? parts[1].type
                                                           : parts[0].type;
      tmp = captureMovesOnly;
      movesList = calculateMoves(playerBoard, opponentBoard, piece, type, color,
                                 &captureMovesOnly);
      if (movesList[0]) {
        if (captureMovesOnly != tmp) {
          for (int j = 0; j < count; j++) {
            moveBoardList[j]->pieceBoard = 0ULL;
            memset(moveBoardList[j]->movesList, 0, 12 * sizeof(board_t));
          }
          count = 0;
        }
        moveBoardList[count]->pieceBoard = piece;
        memcpy(moveBoardList[count]->movesList, movesList,
               12 * sizeof(board_t));
        count++;
      }
    }
  }
  return moveBoardList;
}

void pickNextCaptureMove(board_t *movesList, int depth, board_t move,
                         char *moveString, char type) {
  if (depth > 0) {
    strcat(moveString, colon);
  }
  strcat(moveString, bitBoardToChar(move));
  board_t nextMove;
  board_t tmp;
  for (size_t d = NW; d <= SE; d++) {
    nextMove = shift(shift(move, d), d);
    if (type == 'k') {
      while (!(nextMove & movesList[depth]) &&
             (tmp = shift(shift(nextMove, d), d)))
        nextMove = tmp;
    }
    if (nextMove & movesList[depth]) {
      pickNextCaptureMove(movesList, depth + 1, nextMove, moveString, type);
      return;
    }
  }
}

void pickFirstMove(moveboard_t **moveBoardList, char *moveString) {
  if (moveBoardList[0] != NULL) {
    strcat(moveString, bitBoardToChar(moveBoardList[0]->pieceBoard));
    board_t move;
    for (size_t d = NW; d <= SE; d++) {
      move = shift(moveBoardList[0]->pieceBoard, d);
      if (move & moveBoardList[0]->movesList[0]) {
        // normal move
        strcat(moveString, colon);
        strcat(moveString, bitBoardToChar(move));
        return;
      }
      move = shift(move, d);
      if (move & moveBoardList[0]->movesList[0]) {
        // capture move
        pickNextCaptureMove(moveBoardList[0]->movesList, 1, move, moveString,
                            'n');
        return;
      }
    }
    // has to be king
    pickNextCaptureMove(moveBoardList[0]->movesList, 0,
                        moveBoardList[0]->pieceBoard, moveString, 'k');
  }
}

void print_bitboard(long board, char color) {
  int i;
  char *b = malloc(sizeof(char) * 64);
  for (i = 0; i < 64; i++) {
    b[i] = ' ';
  }
  bitboardToArray(b, board, &color);
  print_board(b);
  free(b);
}

/*
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
  int i;
  for (i = 0; i < 32; i++) {
    int index = allowedSquaresIndices[i];
    int calc_index = parsCoordinateToSquare(BITBOARD_LOOKUP[index]);
    bitboard_t *currentBoard = initBitboard();
    addToBitboard(currentBoard->w, &index, 'w');
    printf("|Calc Square: %d, Square: %d mod square: %d, map to: %s|\n|Current "
           "Board Long: "
           "%ld, Given Board Long: %ld|\n",
           calc_index, index, LONG_BITBOARD_LOOKUP[longOnlyPos[i] % 59],
BITBOARD_LOOKUP[index], currentBoard->w[0].board, longOnlyPos[i]);
    printBitboard(currentBoard);
  }
  bitboard_t *currentBoard = parsFromString(pieceList);
  moveboard_t **moveBoardList = allPossibleMoves(currentBoard, 'w');
  char moveString[35] = {0};
  pickFirstMove(moveBoardList, moveString);
  printf("%s\n", moveString);
  // int i = 0;
  // while (moveBoardList[i++]->pieceBoard) {
  //  print_board(moveBoardList[i]->pieceBoard);
  //  print_board(moveBoardList[i]->movesBoard);
  //}
}
*/