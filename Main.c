#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "performConnection.h"

#define GAME_ID_SIZE 13

int main(int argc, char *argv[])
{

    int sock;
    int c;
    char gameId[GAME_ID_SIZE];
    char playerId[1];

    while ((c = getopt(argc, argv, "g:p:")) != -1)
    {
        switch (c)
        {
        case 'g':
            if (strlen(optarg) != GAME_ID_SIZE)
            {
                printf("wrong game id\n");
                exit(EXIT_FAILURE);
            }
            strcpy(gameId, optarg);
            break;
        case 'p':
        {
            int id;
            if (sscanf(optarg, "%i", &id) < 1 || id < 0 || id > 1)
            {
                printf("wrong player id\n");
                exit(EXIT_FAILURE);
            }
            strcpy(gameId, optarg);
        }
        case '?':
            printf("wrong argument\n");
            exit(EXIT_FAILURE);
            break;

        default:
            break;
        }
    }

    // temporary add game id in Makefile
    if (strlen(gameId) == 0)
    {
        printf("game id is missing\n");
        exit(EXIT_FAILURE);
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("creating socket failed");
        return EXIT_FAILURE;
    }

    performConnection(sock, gameId);

    close(sock);

    return EXIT_SUCCESS;
}
