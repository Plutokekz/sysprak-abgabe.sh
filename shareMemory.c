/** @file shareMemory.c
 * 
 *  @brief SHM fuctions.
 * 
 *  Contains all functions for SHM.
 */
#include "performConnection.h"
#include "shareMemory.h"


/** @brief reserves the memorySize for SHM.
 * The shmID is essencial to identify the SHM.
 */
int createSHM(size_t memorySize) {
    int shmID = shmget(IPC_PRIVATE, memorySize, 0666); //ist 0666 richtig?
    if (shmID < 0) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    return shmID;
}   
/** @brief attaches the reserved memory to the memory of our current process.
 * afterwards, we can work with the returned shm.
 */
void *attachSHM(int shmID) {
    char *shm = shmat(shmID,NULL,0); //Pointer to SHM Adress
    if (shm == (char*) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    return shm;
}

/** @brief detaches the SHM from our current process memory.
 * (Currently unused)
 */

void detachSHM(void *shm) {
    if (shmdt(shm) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
}

/** @brief deletes the SHM identified by shmID and frees the allocated memory.
 */
int deleteSHM(int shmID) {
   int del = shmctl(shmID, IPC_RMID, 0);
   if ( del == -1) {
       perror("shctl, deleteSHM");
       exit(EXIT_FAILURE);
   }
   return del;
}
/** @brief Takes a struct and inserts it into a SHM Segment.
 * 
 * This function is customized to fit the Game Start Data (see Milestone 0).
 * It creates (createSHM) a SHM Segment.
 * Attaches the Segment to the ptrGameStart adress and the fills it with the
 * content of gs.
 * the nested players struct is constructed with a while loop because it must
 * be able to handel a variable amount of players (see Milestone 2).
 */
int setupSHM_GameStart(game_info *gs) {

    //Initialize SHM struct
    struct Share *ptrGameStart;
    size_t sizeOfStruct = sizeof(struct Share) +
    (*gs).total*sizeof(shmPlayer_t);
    int shmID = createSHM(sizeOfStruct);
    ptrGameStart = attachSHM(shmID);

    //Transfer Data to SHM struct
    //TODO Tim: struct von Erik einbinden.
    (*ptrGameStart).gameName = (*gs).gameName;
    printf("in SHM.c, gameName: %s\n", (*ptrGameStart).gameName);
    //(*ptrGameStart).ownPlayerNumber = (*gs).playerList[0].playerId;
    (*ptrGameStart).thinkerPID = getpid();
    (*ptrGameStart).connectorPID = getppid();
    (*ptrGameStart).numberOfPlayers = (*gs).total;

    //Transfer Player Data into nested Player struct;
    /*int numP = (*gs).total; //Anzahl Spieler, Info aus Erik's struct holen
    int p = 0; 
        
    while (p < numP) {
        (*ptrGameStart).players[p].number = p;
        (*ptrGameStart).players[p].name = (*gs).playerList[p].playerName;
        (*ptrGameStart).players[p].readyFlag = (*gs).playerList[p].readyFlag; 
        p++;
    }*/
    return shmID;
}

/** @brief Puts testString into a SHM Segment.
 */
int setupSHM_String(char testString[100]) {
    char *ptrString;
    int shmID = createSHM(100*sizeof(char));
    ptrString = attachSHM(shmID);
    strcpy(ptrString, testString);
    return shmID;
}









