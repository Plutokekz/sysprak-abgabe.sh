/** @file performConnection.h
 *  @brief Function to setup connection to Game Server.
 *
 *  This function implements the prolog phase of the protocol
 *  and parse the server response. It also contains a function
 *  for pretty printing.
 *
 *  @author Erik Kellenter
 *
 * @bug recvCommand throws error when server sends 48 bytes after PLAYER COMMAND.
 *
 */

#ifndef connectionUtil
#define connectionUtil

#include <stdbool.h>

#include "info_structs.h"
#include "modules/share_memory.h"
#include "modules/utils.h"
#include "modules/log.h"
#include "modules/protocol_utils.h"

#define GAME_ID_SIZE 13
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define GAMEKINDNAME "Bashni"
#define CLIENT_VERSION "2.42"

/** @brief implements prolog phase using passed Opt.
 *
 *  @param sock socket to connect with
 *  @param config options used for the connection
 */
void performConnection(int sock, config_t *config);

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
