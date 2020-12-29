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

#define BUFF_SIZE 100

typedef struct Player {
    int Number;
    char *Name;
    int ReadyFlag;

} Player;


typedef struct Share {
    
    char* gameName;
    int gameNumber;
    int numberOfPlayer;
    pid_t thinkerPID; //nicht 100% sicher, evtl muss das int sein
    pid_t connectorPID;
    struct Player players[]; //2. playerData nötig?

} Share; //oder Share_t?

int createSHM(size_t memorySize);

void *attachSHM(int shmID);

void detachSHM(void *shm);

int deleteSHM(int shmID);

#endif