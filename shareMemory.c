#include "shareMemory.h"


int createSHM(struct varStruct) {
    int shmID;
    shmID = shmget(IPC_PRIVATE, sizeof(varStruct));
    if (shmID < 0) {
        perror("shmget"):
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

}

void detachSHM(void *shm) {
    if (shmdt(shm) == -1) {
        perror("shmdt");
        exit(EXIT_FAILURE);
    }
}

int deleteSHM(int shmID) {
   if (int del = shmctl(shmID, IPC_RMID, 0) == -1) {
       perror("shctl, deleteSHM")
   }
   return del;
}




