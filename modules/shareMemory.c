
#include "shareMemory.h"

int createSHM(size_t memorySize) {
    int shmID = shmget(IPC_PRIVATE, memorySize, 0666); //ist 0666 richtig?
    if (shmID < 0) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    return shmID;
}   

void *attachSHM(int shmID) {
    char *shm = shmat(shmID,NULL,0); //Pointer to SHM Adress
    if (shm == (char*) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    return shm;
}

void detachSHM(void *shm) {
    if (shmdt(shm) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
}


int deleteSHM(int shmID) {
   int del = shmctl(shmID, IPC_RMID, 0);
   if ( del == -1) {
       perror("shctl, deleteSHM");
       exit(EXIT_FAILURE);
   }
   return del;
}

int setupSHM_GameStart(game_info *gs) {

    //Initialize SHM struct
    struct Share *ptrGameStart;
    size_t sizeOfStruct = sizeof(struct Share) +
    (*gs).total*sizeof(shmPlayer_t)+(500*sizeof(char));
    int shmID = createSHM(sizeOfStruct);
    ptrGameStart = attachSHM(shmID);

    printf("shmID in ShareMem.c: %d\n", shmID);

    //Transfer Data to SHM struct
    //TODO Tim: struct von Erik einbinden.
    strcpy(ptrGameStart->gameName, gs->gameName);
    ptrGameStart->ownPlayerNumber = gs->playerList[0]->playerId;
    ptrGameStart->thinkerPID = getpid();
    ptrGameStart->connectorPID = getppid();
    ptrGameStart->numberOfPlayers = gs->total;
    ptrGameStart->thinkerGuard = 1; //TODO auf 0 setzen und entscheiden,
    //wo wir Guard = 1 setzen wollen.

    //Transfer Player Data into nested Player struct;
    int numP = (*gs).total; //Anzahl Spieler, Info aus Erik's struct holen
    int p = 0; 
        
    while (p < numP) {
        ptrGameStart->players[p].number = p;
        strcpy(ptrGameStart->players[p].name, gs->playerList[p]->playerName);
        ptrGameStart->players[p].readyFlag = gs->playerList[p]->ready;
        p++;
    }
    return shmID;
}


int setupSHM_String(char testString[100]) {
    char *ptrString;
    int shmID = createSHM(100*sizeof(char));
    ptrString = attachSHM(shmID);
    strcpy(ptrString, testString);
    return shmID;
}









