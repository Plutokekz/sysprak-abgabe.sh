/** @file performConnection.h
 *  @brief Function to setup connection to Game Server.
 *
 *  This function implements the prolog phase of the protocol
 *  and parse the server response. It also contains a function
 *  for pretty printing.
 *
 *  @author Erik Kellenter
 *  @bug recvCommand throws error when server sends 48 bytes after PLAYER
 * COMMAND.
 */

#ifndef connectionUtil
#define connectionUtil

#include <stdbool.h>

#define GAME_ID_SIZE 13

/**
 * @brief Config struct
 *
 * Contains all the information passed as command-line arguments.
 */
typedef struct Opt {
  char gameId[14];
  char playerId[2];
} opt_t;

/**
 * @brief Config struct
 *
 * Describes a player sent by the server.
 */
typedef struct Player {
  char playerName[30];
  unsigned int playerId;
  bool ready;
} player_t;

/**
 * @brief Config struct
 *
 * Contains all relevant information of the prolog phase.
 * Don't forget to free memory after usage.
 */
typedef struct GameInfo {
  char serverVersion[10];
  char gamekindName[20];
  char gameName[30];
  unsigned int total;
  player_t **playerList;
  char *raw;
} game_info;

/**
 * @brief Config struct
 *
 * Contains all relevant commands for following functions.
 */
typedef enum COMMAND { RETURN, START, VERSION, ID, PLAYER } COMMAND;

/**
 * @brief Config struct
 *
 * Contains the flags for the function printProlog.
 */
typedef enum P_FLAG { PRETTY, DEBUG } P_FLAG;

/** @brief implements prolog phase using passed Opt.
 *
 *  @param sock socket to connect with
 *  @param opt options used for the connection
 */
void performConnection(int sock, opt_t *opt);

/** @brief Sends command to server.
 *
 * The function parses and concatenates the command with the value
 * passed to the function and afterwards send it to the server.
 *
 *  @param c enum COMMAND
 *  @param value value that will be appended to command
 */
void sendCommand(COMMAND c, char *value);

/** @brief Receives server response after sending a command.
 *
 * The function receives the number of lines passed as parameter.
 * If lines is 0, then the function waits until an END command
 * (e. g. ENDPLAYERS or ENDPIECELIST) is received. It returns
 * a pointer to a dynamically allocated memory that must be freed.
 *
 *  @param lines expected lines the server will send
 *  @return pointer to received data
 */
void *recvCommand(int lines);

/** @brief getter for game_info struct.
 *
 * To avoid memory leaks, call freeGameInfo afterwards.
 *
 *  @return game_info struct
 */
game_info *getGameInfo();

/** @brief frees memory allocated before.
 *
 *  @param gameInfo game_info struct that has been used before
 */
void freeGameInfo(game_info *gameInfo);

#endif