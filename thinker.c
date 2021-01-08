#include "shareMemory.h"
#include "thinker.h"


void thinker() {

    //Thinker*************
    int shmID;
    close(fd[1]); //entweder var in perfC oder shmID ausgeben aus perfC
    if (read(fd[0], &shmID, sizeof(int)) < 0) {
      perror("Error writing to pipe");
    }

    struct Share *ptrGameStart, gameStart;
    ptrGameStart = &gameStart;
    ptrGameStart = attachSHM(shmID);
    printf("in thinker(), Erfolg: %s\n",(*ptrGameStart).gameName);
    deleteSHM(shmID);

}







 


