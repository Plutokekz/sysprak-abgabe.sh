#ifndef BASCHNI_SHM_H
#define BASCHNI_SHM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BUFF_SIZE 100

typedef struct PlayerData {
    int PlayerNumber;
    char* PlayerName;
    int PlayerReadyFlag;

} PlayerData;


typedef struct Share {
    char* gameName;
    int gameNumber;
    int numberOfPlayer;
    pid_t thinkerPID; //nicht 100% sicher, evtl muss das int sein
    pid_t connectorPID;
    struct PlayerData PlayerData; //2. playerData nötig?

} Share; //oder Share_t?



#endif