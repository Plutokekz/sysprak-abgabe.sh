#include "shareMemory.h"

int main(int argc, char *argv[]) {
    pid_t pid;
    int fd[2];
    int shmID;
    int key = 1234;
    //size_t size = sizeof(struct Share)+
    //sizeof(size_t)+20*(sizeof(int))+2*(sizeof(pid_t));
    //große size deklarieren zum testen

	if (pipe(fd) < 0) {
		perror("Fehler bei Pipe");
		exit(EXIT_FAILURE);
	}
    if ((pid = fork()) < 0) {
		perror("Fehler bei Fork");
		exit(EXIT_FAILURE);
	} else if (pid > 0) { //Elternprozess
        close(fd[1]); //Schreib Pipe Schließen
        size_t pSize;


    	if ((waitpid (pid, NULL, 0)) < 0) {
			perror ("Fehler beim Warten auf Kindprozess.");
			exit (EXIT_FAILURE);
		}
        read(fd[0], &shmID, sizeof(int));
        read(fd[0], &pSize, sizeof(size_t)); //size of struct
        
        printf("Size of pSize (parent, after pipe): %zu\n", pSize);//TEMP
        
        //Create Struct
        struct Share *ptrGameStart, gameStart;
        ptrGameStart = &gameStart;
        printf("Size of ptrGS after creation: %zu\n", sizeof(*ptrGameStart));
        //Read SHM
        shmID = shmget(key,pSize, 0666);
        printf("shmID Parent: %d\n", shmID);
        ptrGameStart = attachSHM(shmID);
        printf("Size of ptrGS after reading SHM: %zu\n", sizeof(*ptrGameStart));
        
        
        //Testen
        printf("Erfolg: %s\n", (*ptrGameStart).gameName);
        printf("GameNumber: %d\n", (*ptrGameStart).gameNumber);
        printf("NumberOfPlayers: %d\n", (*ptrGameStart).numberOfPlayer);
        deleteSHM(shmID);  
        exit(EXIT_SUCCESS); 
     
    } else { //Kindprozess
        close(fd[0]); //close read
        struct Share *ptrGameStart, gameStart;
        ptrGameStart = &gameStart;
        printf("size of ptrGameStart: %zu\n", sizeof(*ptrGameStart));
        
        //struct befüllen

        printf("ptrGS befüllt, gameName: %s\n", (*ptrGameStart).gameName);
        printf("size of ptrGameStart after input: %zu\n", sizeof(*ptrGameStart));
        size_t sizeOfStruct = sizeof(*ptrGameStart)+(100*sizeof(int));

        //Shared Memory
        shmID = shmget(key,sizeOfStruct, IPC_CREAT | 0666);
        printf("shmID Child: %d\n", shmID);
        ptrGameStart = (struct Share*) (void*) attachSHM(shmID); 
        //memcpy(gameStartPtr, &gameStart, size);
        (*ptrGameStart).gameName = "SHM Funktioniert!";
        (*ptrGameStart).gameNumber = 5;
        (*ptrGameStart).thinkerPID = getpid();
        (*ptrGameStart).connectorPID = pid;
        (*ptrGameStart).numberOfPlayer = 2;
        printf("size of size_t: %ld\n", sizeof(size_t));
        //mem ptr in pipe
        write(fd[1], &shmID, sizeof(int));
        write(fd[1], &sizeOfStruct, sizeof(size_t));
        close(fd[1]);
    } 
}


//struct person *personPtr, person1;