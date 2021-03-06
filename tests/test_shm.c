#include "../modules/my_signal.h"
#include "../modules/share_memory.h"

void my_handler(int signum) {
    if (signum == SIGUSR1) {
        printf("Received SIGUSR1!\n");
    }
}

void handleSig1 (int sig) {
    //think();
    printf("Signal received");
}

int main() {
    pid_t pid;
    int fd[2];
    int shmID;
    int shmID2;

	if (pipe(fd) < 0) {
		perror("Fehler bei Pipe");
		exit(EXIT_FAILURE);
	}
    if ((pid = fork()) < 0) {
		perror("Fehler bei Fork");
		exit(EXIT_FAILURE);
	} else if (pid > 0) { //Elternprozess
        close(fd[1]); //Schreib Pipe Schließen

        //Wait for Child
    	if ((waitpid (pid, NULL, 0)) < 0) {
			perror ("Fehler beim Warten auf Kindprozess.");
			exit (EXIT_FAILURE);
		}
        read(fd[0], &shmID, sizeof(int));
        read(fd[0], &shmID2, sizeof(int));

        //Create Struct
        struct Share *ptrGameStart, gameStart;
        ptrGameStart = &gameStart;
        ptrGameStart = attachSHM(shmID);

        //Testen
        printf("Erfolg: %s\n", (*ptrGameStart).gameName);
        printf("GameNumber: %d\n", (*ptrGameStart).ownPlayerNumber);
        printf("NumberOfPlayers: %d\n", (*ptrGameStart).numberOfPlayers);
        printf("Pid Parent(Thinker): %d, Pid Child: %d\n",
        (*ptrGameStart).thinkerPID, (*ptrGameStart).connectorPID);
        printf("Player 0: %s\n", (*ptrGameStart).players[0].name);
        printf("Player 1: %s\n", (*ptrGameStart).players[1].name);


        //Test Signal



        signal(SIGUSR1, my_handler);




        struct sigaction sa = { 0 };
        sa.sa_flags = SA_RESTART;
        sa.sa_handler = &handleSig1;
        sigaction(SIGUSR1, &sa, NULL);


        //Test setupSHM_String
        char *ptrString;
        ptrString = attachSHM(shmID2);
        printf("Your String: %s\n", ptrString);

        close(fd[0]);
        deleteSHM(shmID);
        deleteSHM(shmID2);
        exit(EXIT_SUCCESS);
        sleep(10);
        signal(SIGUSR1, my_handler);


    } else { //Kindprozess
        close(fd[0]); //close read


        //Test Signal**



        struct Share ptrTestGS;
        strcpy(ptrTestGS.gameName, "SHM Funktioniert!");
        ptrTestGS.ownPlayerNumber = 42;
        ptrTestGS.numberOfPlayers = 2;
        ptrTestGS.players[0].number = 0;
        strcpy(ptrTestGS.players[0].name, "Anna");
        ptrTestGS.players[0].readyFlag = 1;
        ptrTestGS.players[1].number = 1;
        strcpy(ptrTestGS.players[1].name, "Ben");
        ptrTestGS.players[1].readyFlag = 1;
        shmID = setupSHM_GameStart((game_info *)&ptrTestGS);

        //100 ist temporär, können wir uns noch überlegen wie groß das sein muss
        char testString[100] = "setupSHM_String funktioniert :)";
        shmID2 = setupSHM_String(testString);

        write(fd[1], &shmID, sizeof(int));
        write(fd[1], &shmID2, sizeof(int));
        close(fd[1]);
        sleep(3);
        kill(getppid(), SIGUSR1);
        printf("sending signal to Parent\n");
    }
}


