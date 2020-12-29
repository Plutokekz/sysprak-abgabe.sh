#ifndef BASCHNI_SIGNAL_H
#define BASCHNI_SIGNAL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

typedef struct PlayerData {
    int PlayerNumber;
    char* PlayerName;
    int PlayerReadyFlag;

} PlayerData;


typedef struct Share {
    
    char* gameName;
    int gameNumber;
    int numberOfPlayer;
    pid_t thinkerPID;
    pid_t connectorPID;
    //struct PlayerData PlayerData; //2. playerData nötig?

} Share; 



#endif