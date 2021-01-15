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

int createSHM(size_t memorySize);

void *attachSHM(int shmID);

void detachSHM(void *shm);

int deleteSHM(int shmID);

int setupSHM_String(char *ptrString);

int setupSHM_GameStart(game_info *gs);

#endif