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
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <limits.h>

#include "performConnection.h"

typedef struct shmPlayer {
    int number;
    char *name;
    bool readyFlag;

} shmPlayer_t;


typedef struct Share {
    
    char* gameName;
    int ownPlayerNumber;
    int numberOfPlayers;
    pid_t thinkerPID;
    pid_t connectorPID;
    int thinkerGuard;
    shmPlayer_t players[];

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