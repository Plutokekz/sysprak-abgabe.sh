//
// Created by Lukas on 19.01.2021.
//

#include "bitboard.h"
char *rows = "ABCDEFGH";
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

int allowedSquaresIndices[32] = {0,  2,  4,  6,  9,  11, 13, 15, 16, 18, 20,
                                 22, 25, 27, 29, 31, 32, 34, 36, 38, 41, 43,
                                 45, 47, 48, 50, 52, 54, 57, 59, 61, 63};

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

void bitboardToArray(char *b, long long bitboard, char color) {
  int i, j;
  for (i = 7; i >= 0; i--) {
    for (j = 0; j < 8; j++) {
      if (get(bitboard, (j * 8 + i))) {
        b[j * 8 + i] = color;
      }
    }
  }
}

int parsCoordinateToSquare(char *coord) {
  // int row = *(coord) - 'A'; // char coord to int index
  // int column = *(coord+1) - '1'; // char number to int
  // printf("Current Piece: %s, calc coords: row: %d, column: %d\n", coord, row,
  // column);
  return (*(coord) - 'A') * 8 + (*(coord + 1) - '1');
}

void printBitboard(bitboard_t *board) {
  char *b = malloc(sizeof(char) * 64);
  for (int i = 0; i < 64; i++) {
    b[i] = ' ';
  }
  bitboardToArray(b, board->whiteBoard, 'w');
  bitboardToArray(b, board->whiteQueenBoard, 'W');
  bitboardToArray(b, board->blackBoard, 'b');
  bitboardToArray(b, board->blackQueenBoard, 'B');
  print_board(b);
  free(b);
}

bitboard_t *parsFromString(char *pieceList) {
  initTowerBoard(TOWER_BOARD);
  bitboard_t *board = calloc(1, sizeof(bitboard_t));
  pieceList = strchr(pieceList, '\n') + 1; // skipp + PIECESLIST 24\n
  char pieceString[4];                     // e.g. b@F8
  for (size_t i = 0; i < NUMBER_OF_PIECES; i++) {
    memcpy(pieceString, pieceList + 2, 4);
    piece_t *piece = calloc(1, sizeof(piece_t));
    piece->next = NULL;
    switch (pieceString[0]) {
    case 'b':
      piece->color = 'b';
      piece->type = 'n';
      break;
    case 'B':
      piece->color = 'b';
      piece->type = 'k';
      break;
    case 'w':
      piece->color = 'w';
      piece->type = 'n';
      break;
    case 'W':
      piece->color = 'w';
      piece->type = 'k';
      break;
    default:
      printf("Error color and type <%c> does not exist\n", *pieceString);
    }
    int index = parsCoordinateToSquare(pieceString + 2);
    pushPiece(TOWER_BOARD[index], piece);
    pieceList += SIZE_OF_PIECE;
  }
  for (size_t i = 0; i < 32; i++) {
    tower_t *tower = TOWER_BOARD[allowedSquaresIndices[i]];
    if (getColor(tower) == 'b') {
      if (getType(tower) == 'n') {
        board->blackBoard = set(board->blackBoard, allowedSquaresIndices[i]);
      } else {
        board->blackQueenBoard =
            set(board->blackQueenBoard, allowedSquaresIndices[i]);
      }
    } else if (getColor(tower) == 'w') {
      if (getType(tower) == 'n') {
        board->whiteBoard = set(board->whiteBoard, allowedSquaresIndices[i]);
      } else {
        board->whiteQueenBoard =
            set(board->whiteQueenBoard, allowedSquaresIndices[i]);
      }
    }
  }
  board->emptyBoard = ~(board->blackBoard | board->blackQueenBoard |
                        board->whiteBoard | board->whiteQueenBoard);
  return board;
}

DIRECTION turnAroundDirection(DIRECTION d) {
  switch (d) {
  case NW:
    return SE;
  case NE:
    return SW;
  case SW:
    return NE;
  case SE:
    return NW;
  default:
    printf("wrong direction\n");
    return 0;
  }
}

board_t shift(board_t piece, DIRECTION d) {
  switch (d) {
  case NW:
    return (piece >> (COLUMNS - 1)) & ALLOWED_SQUARES;
  case NE:
    return (piece << (COLUMNS + 1)) & ALLOWED_SQUARES;
  case SW:
    return (piece >> (COLUMNS + 1)) & ALLOWED_SQUARES;
  case SE:
    return (piece << (COLUMNS - 1)) & ALLOWED_SQUARES;
  default:
    printf("wrong direction\n");
    return 0ULL;
  }
}

void captureMoves(board_t playerBoard, board_t opponentBoard,
                  board_t emptyBoard, board_t piece, char type, char color,
                  board_t *movesList, int depth, DIRECTION lockedDirection) {
  board_t move;
  board_t tmp;
  for (size_t d = NW; d <= SE; d++) {
    if (d != lockedDirection) {
      if (type == 'k') {
        tmp = piece;
        board_t start = piece;
        move = piece;
        while ((tmp = shift(move, d)) & emptyBoard)
          move = tmp;
        if (shift((shift(move, d) & opponentBoard), d) & emptyBoard) {
          tower_t *oldTower, *newTower;
          oldTower = getTower(start);
          newTower = getTower(move);
          moveTower(&oldTower, &newTower);
          playerBoard = (playerBoard & ~start) | move;
          emptyBoard = ~(playerBoard | opponentBoard);
          piece = move;
        }
      }
      move = shift((shift(piece, d) & opponentBoard), d) & emptyBoard;
      if (move) {
        tower_t *oldTower, *newTower;
        tmp = piece;
        oldTower = getTower(tmp);
        newTower = getTower(move);
        board_t captured = shift(move, turnAroundDirection(d));
        char capturedTowerColor =
            capture(&oldTower, &newTower, getTower(captured));
        playerBoard = (playerBoard & ~tmp) | move;
        opponentBoard = opponentBoard & ~captured;
        if (capturedTowerColor == color) {
          playerBoard |= captured;
        } else if (capturedTowerColor != 0) {
          opponentBoard |= captured;
        }
        emptyBoard = ~(playerBoard | opponentBoard);
        movesList[depth] |= move;
        if ((color == 'b' && (move & WHITE_BASELINE)) ||
            (color == 'w' && (move & BLACK_BASELINE)))
          type = 'k';
        captureMoves(playerBoard, opponentBoard, emptyBoard, move, type, color,
                     movesList, depth + 1, turnAroundDirection(d));
        break;
      }
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
                 movesList, 0, UNDEFINED);
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
    captureMoves(playerBoard, opponentBoard, emptyBoard, piece, type, color,
                 movesList, 0, UNDEFINED);
    if (movesList[0] == 0ULL && !(*captureMovesOnly)) {
      board_t move;
      board_t nonCaptureMoves = 0ULL;
      board_t tmp;
      for (size_t d = NW; d <= SE; d++) {
        move = piece;
        while ((tmp = shift(move, d)) & emptyBoard) {
          move = tmp;
          nonCaptureMoves |= move;
        }
      }
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
  board_t playerBoard;
  board_t opponentBoard;
  if (color == 'w') {
    playerBoard = currentBoard->whiteBoard | currentBoard->whiteQueenBoard;
    opponentBoard = currentBoard->blackBoard | currentBoard->blackQueenBoard;
  } else {
    playerBoard = currentBoard->blackBoard | currentBoard->blackQueenBoard;
    opponentBoard = currentBoard->whiteBoard | currentBoard->whiteQueenBoard;
  }
  board_t piece;
  board_t *movesList;
  int count = 0;
  char type;
  bool captureMovesOnly = false;
  bool tmp;
  for (size_t i = 0; i < 32; i++) {
    if ((piece = get(playerBoard, allowedSquaresIndices[i]))) {
      type = getType(getTower(piece));
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
          // only look for first capture move
          moveBoardList[count]->pieceBoard = piece;
          memcpy(moveBoardList[count]->movesList, movesList,
                 12 * sizeof(board_t));
          return moveBoardList;
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
                         char *moveString) {
  if (depth > 0) {
    strcat(moveString, colon);
  }
  strcat(moveString, bitBoardToChar(move));
  board_t nextMove;
  board_t tmp;
  for (size_t d = NW; d <= SE; d++) {
    nextMove = shift(shift(move, d), d);
    while (!(nextMove & movesList[depth]) && (tmp = shift(nextMove, d)))
      nextMove = tmp;
    if (nextMove & movesList[depth]) {
      pickNextCaptureMove(movesList, depth + 1, nextMove, moveString);
      return;
    }
  }
}

void pickFirstMove(moveboard_t **moveBoardList, char *moveString) {
  if (moveBoardList[0] != NULL) {
    board_t move;
    for (size_t d = NW; d <= SE; d++) {
      move = shift(moveBoardList[0]->pieceBoard, d);
      if (move & moveBoardList[0]->movesList[0]) {
        // normal move
        strcat(moveString, bitBoardToChar(moveBoardList[0]->pieceBoard));
        strcat(moveString, colon);
        strcat(moveString, bitBoardToChar(move));
        return;
      }
    }
    pickNextCaptureMove(moveBoardList[0]->movesList, 0,
                        moveBoardList[0]->pieceBoard, moveString);
  }
}

void print_bitboard(long board, char color) {
  int i;
  char *b = malloc(sizeof(char) * 64);
  for (i = 0; i < 64; i++) {
    b[i] = ' ';
  }
  bitboardToArray(b, board, color);
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
                     "+ b@A7\n"
                     "+ b@H6\n"
                     "+ b@F6\n"
                     "+ b@D6\n"
                     "+ b@B6\n"
                     "+ w@E5\n"
                     "+ b@E5\n"
                     "+ w@G3\n"
                     "+ w@E3\n"
                     "+ w@C3\n"
                     "+ w@H2\n"
                     "+ w@F2\n"
                     "+ w@D2\n"
                     "+ w@B2\n"
                     "+ w@G1\n"
                     "+ w@E1\n"
                     "+ w@C1\n"
                     "+ w@A1\n"
                     "+ ENDPIECESLIST";
  char pieceList[] = "PIECESLIST 24\n+ b@F8\n+ b@F8\n+ W@F8\n+ b@H6\n+ b@D6\n+ "
                     "w@D6\n+ W@D6\n+ b@E5\n+ b@E5\n+ b@E5\n+ b@C5\n+ b@C5\n+ "
                     "w@C5\n+ b@D4\n+ b@D4\n+ w@D4\n+ w@B4\n+ w@A3\n+ w@H2\n+ "
                     "b@F2\n+ w@F2\n+ w@B2\n+ w@C1\n+ w@A1\n+ ENDPIECESLIST";
  bitboard_t *currentBoard = parsFromString(pieceList);
  printBitboard(currentBoard);
  moveboard_t **moveBoardList = allPossibleMoves(currentBoard, 'b');
  char moveString[35] = {0};
  pickFirstMove(moveBoardList, moveString);
  printf("%s\n", moveString);
}
*/