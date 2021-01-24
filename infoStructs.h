//
// Created by Lukas on 20.01.2021.
//

#ifndef BASCHNI_INFOSTRUCTS_H
#define BASCHNI_INFOSTRUCTS_H

#include <stdbool.h>


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
#endif // BASCHNI_INFOSTRUCTS_H
