#include "shareMemory.h"

int main(int argc, char *argv[]) {
    pid_t pid;
    int fd[2];
    int shmID;
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

        //Wait for Child
    	if ((waitpid (pid, NULL, 0)) < 0) {
			perror ("Fehler beim Warten auf Kindprozess.");
			exit (EXIT_FAILURE);
		}
        read(fd[0], &shmID, sizeof(int));
        
        printf("Size of pSize (parent, after pipe): %zu\n", pSize);//TEMP
        
        //Create Struct
        struct Share *ptrGameStart, gameStart;
        ptrGameStart = &gameStart;
        printf("Size of ptrGS after creation: %zu\n", sizeof(*ptrGameStart));
        printf("shmID Parent: %d\n", shmID);
        ptrGameStart = attachSHM(shmID);
        printf("Size of ptrGS after reading SHM: %zu\n", sizeof(*ptrGameStart));
        
        
        //Testen
        printf("Erfolg: %s\n", (*ptrGameStart).gameName);
        printf("GameNumber: %d\n", (*ptrGameStart).gameNumber);
        printf("NumberOfPlayers: %d\n", (*ptrGameStart).numberOfPlayer);
        printf("Pid Parent(Thinker): %d, Pid Child: %d\n",
        (*ptrGameStart).thinkerPID, (*ptrGameStart).connectorPID);
        printf("Parent!!: Nummer: %d, Name : %s\n", (*ptrGameStart).players[1].Number,
             (*ptrGameStart).players[1].Name);


        close(fd[0]);
        deleteSHM(shmID);
        exit(EXIT_SUCCESS); 
     
    } else { //Kindprozess
        close(fd[0]); //close read
        struct Share *ptrGameStart, gameStart;
        ptrGameStart = &gameStart;
        printf("size of ptrGameStart: %zu\n", sizeof(*ptrGameStart));
        
        

        
        size_t sizeOfStruct = sizeof(char);
        //+sizeof(char[100])+(2*sizeof(int)); wie muss ich größe wählen?

        //Shared Memory
        shmID = createSHM(sizeOfStruct);
        printf("shmID Child: %d\n", shmID);
        ptrGameStart = attachSHM(shmID); 

        //struct befüllen
        (*ptrGameStart).gameName = "SHM Funktioniert!";
        (*ptrGameStart).gameNumber = 5;
        (*ptrGameStart).thinkerPID = getpid();
        (*ptrGameStart).connectorPID = getppid();
        (*ptrGameStart).numberOfPlayer = 2;

                //____________playerData struct_______________ 
        //infos vom server 
        int numP = 2; //Mitspierleranzahl
        int p = 0;
        int pNumber[numP];
        char *pName[numP];
        int pReadyFlag[numP]; 
        
        while (p < numP) {
            //ptrPlayers[p] = &players[p];
            (*ptrGameStart).players[p].Number = p;
            (*ptrGameStart).players[p].Name = "Franz";
            (*ptrGameStart).players[p].ReadyFlag = 1;
            //lel das funtkionier...guess i m just THAT good :D
            printf("Nummer: %d, Name : %s\n", (*ptrGameStart).players[p].Number,
             (*ptrGameStart).players[p].Name);
            
            p++;
        } 
                //______________________________________________
        
        printf("size of size_t: %ld\n", sizeof(size_t));
        write(fd[1], &shmID, sizeof(int));
        close(fd[1]);
    } 
}


//struct person *personPtr, person1;