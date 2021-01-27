/** @file shareMemory.c
 * @brief SHM fuctions.
 *
 * Contains all functions for SHM.
 *
 * @bug can't output ptrGameStart->gameName in parent process
 */
#ifndef BASCHNI_SHM_H
#define BASCHNI_SHM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#include <fcntl.h>
#include <limits.h>
#include <stdbool.h>
#include <sys/wait.h>
#include <unistd.h>

#include "../infoStructs.h"

/** @brief Struct for all Players
 *
 */
typedef struct shmPlayer {
  int number;
  char name[30];
  bool readyFlag;

} shmPlayer_t;

/** @brief SHM Struct, which contains all necessary infos for thinker().
 *
 * @param gameName custom name of the created Game (e.g. GameTim)
 * @param ownPlayerNumber The Players own number (1 or 2)
 * @param numberOfPlayers Total number of players (usually 2)
 * @param thinkerGuard See MS3: "Flag...das nur gesetzt ist,
 * wenn der Thinker auch wirklich einen neuen Zug liefern soll", is set to 1 by
 * gamePhase.c
 * @param players contains information for each player
 */
typedef struct Share {

  char gameName[30];
  int ownPlayerNumber;
  int numberOfPlayers;
  pid_t thinkerPID;
  pid_t connectorPID;
  int thinkerGuard;
  shmPlayer_t players[2];

} Share;

/** @brief reserves the memorySize for SHM.
 *
 * The shmID is essencial to identify the SHM.
 */
int createSHM(size_t memorySize);

/** @brief attaches the reserved memory to the memory of our current process.
 * afterwards, we can work with the returned shm.
 */
void *attachSHM(int shmID);

/** @brief detaches the SHM from our current process memory.
 * (Currently unused)
 */
void detachSHM(void *shm);

/** @brief deletes the SHM identified by shmID and frees the allocated memory.
 */
int deleteSHM(int shmID);

/** @brief Takes a struct and inserts it into a SHM Segment.
 *
 * This function is customized to fit the Game Start Data (see Milestone 0).
 * It creates (createSHM) a SHM Segment.
 * Attaches the Segment to the ptrGameStart adress and the fills it with the
 * content of gs.
 * the nested players struct is constructed with a while loop because it must
 * be able to handel a variable amount of players (see Milestone 2).
 */
int setupSHM_String(char *ptrString);

/** @brief Puts testString into a SHM Segment.
 *
 */
int setupSHM_GameStart(game_info *gs);

#endif