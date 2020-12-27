#include "shareMemory.h"

int main(int argc, char *argv[]) {
    pid_t pid;
    int fd[2];
    int shmID;
    size_t size = sizeof(struct Share)+
    sizeof(size_t)+2*(sizeof(int))+2*(sizeof(pid_t))+(size_t)1000;
    //große size deklarieren

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
        
        printf("Size of size (parent, after pipe): %zu\n", pSize);//TEMP
        struct Share *shm = attachSHM(shmID);
        struct Share gameStart2;
        memcpy(&gameStart2, shm, (sizeof(size))); //copy struct from shm into gameStart2
        
        //Testen
        printf("Erfolg: %s\n", gameStart2.gameName);
        printf("GameNumber: %d\n", gameStart2.gameNumber);
        printf("NumberOfPlayers: %d\n", gameStart2.numberOfPlayer);
        deleteSHM(shmID);  
        exit(EXIT_SUCCESS); 
     
    } else { //Kindprozess
        close(fd[0]); //close read
        struct Share *gameStartPtr, gameStart;
        gameStartPtr = &gameStart;
        printf("size of gameStart: %zu\n", sizeof(gameStart));
        gameStart.gameName = "SHM Funktioniert!";
        printf("size of gameName: %zu\n", sizeof(gameStart.gameName));
        gameStart.gameNumber = 5;
        gameStart.thinkerPID = getpid();
        gameStart.connectorPID = pid;
        gameStart.numberOfPlayer = 2;
    
        printf("size of gameStart after input: %zu\n", sizeof(gameStart));
        
        printf("Size of gameStart_size: %zu\n", size);
        shmID = createSHM(size);
        gameStartPtr = attachSHM(shmID); 
        memcpy(gameStartPtr, &gameStart, size);

        //mem ptr in pipe
        write(fd[1], &shmID, sizeof(int));
        write(fd[1], &size, sizeof(size));
        close(fd[1]);
    } 
}


//struct person *personPtr, person1;