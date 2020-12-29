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

    //memcpy(shm, varStruct, sizeof(varStruct)); //save varStruct in shm



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




