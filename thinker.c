#include "shareMemory.h"
#include "mySignal.h"


void thinker() {

    close(fd[1]);
    if (write(fd[1], &shmID, sizeof(int)) < 0) {
        perror("Error writing to pipe");
    }
}







 


