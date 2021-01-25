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

#include "config.h"
#include "protocolUtil.h"

#define GAME_ID_SIZE 13
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define GAMEKINDNAME "Bashni"
#define CLIENT_VERSION "2.42"

/**
 * @brief Opt struct
 * \struct Opt
 *
 * Contains all the information passed as command-line arguments.
 */
typedef struct Opt {
  char gameId[14];
  char playerId[2];
} opt_t;

/**
 * @brief Player struct
 * \struct Player
 *
 * Describes a player sent by the server.
 */
typedef struct Player {
  char playerName[30];
  unsigned int playerId;
  bool ready;
} player_t;

/**
 * @brief GameInfo struct
 * \struct GameInfo
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
 * @brief P_FLAG enum
 * \enum P_FLAG
 *
 * Contains the flags for the function printProlog.
 */
typedef enum P_FLAG { PRETTY, DEBUG } P_FLAG;

/** @brief implements prolog phase using passed Opt.
 *
 *  @param sock socket to connect with
 *  @param opt options used for the connection
 */
void performConnection(int sock, opt_t *opt, config_t *config, P_FLAG f);

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