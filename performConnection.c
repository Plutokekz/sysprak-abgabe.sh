#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include "performConnection.h"

#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define GAMEKINDNAME "Bashni"
#define CLIENT_VERSION "2.42"
#define BUFF_SIZE 100

void recvCommand(int sock, char buff[BUFF_SIZE])
{
    ssize_t bytes = 0;

    while (buff[bytes - 1] != '\n' && BUFF_SIZE > bytes)
    {
        ssize_t tmp;
        switch ((tmp = recv(sock, buff + bytes, (BUFF_SIZE - bytes) - 1, 0)))
        {
        case -1:
            perror("receive error");
            exit(EXIT_FAILURE);
        case 0:
            printf("connection closed by server\n");
            exit(EXIT_FAILURE);

        default:
            bytes += tmp; // increment returned bytes every loop
            break;
        }
    }
    buff[bytes] = '\0';

    // error handling
}

// return size for s
size_t formatCommand(char buff[BUFF_SIZE], char *command, char *value)
{
    memset(buff, 0, BUFF_SIZE);
    int rv;

    if (strlen(value) == 0)
    {
        rv = sprintf(buff, "%s\n", command);
    }
    else
    {
        rv = sprintf(buff, "%s %s\n", command, value);
    }

    if (rv > 0)
        return rv;
    printf("command formatting error\n");
    exit(EXIT_FAILURE);
}

void performConnection(int sock, gameOpt *opt)
{
    in_addr_t addr;
    struct sockaddr_in server;
    struct hostent *host;

    host = gethostbyname(HOSTNAME); // upgrade to better function later

    memcpy(&server.sin_addr, host->h_addr_list[0], sizeof(addr));

    server.sin_family = AF_INET; // upgrade to IPv6 later
    server.sin_port = htons(PORTNUMBER);

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("connection failed");
        exit(EXIT_FAILURE);
    }

    char recvBuff[BUFF_SIZE];
    char sendBuff[BUFF_SIZE];
    size_t size;

    recvCommand(sock, recvBuff); // + MNM Gameserver <<Gameserver Version>> accepting connections
    printf("S: %s", recvBuff);

    size = formatCommand(sendBuff, "VERSION", CLIENT_VERSION);
    if (send(sock, sendBuff, size, 0) > 0)
    {
        printf("C: %s", sendBuff);
    }
    else
    {
        exit(EXIT_FAILURE);
    }

    recvCommand(sock, recvBuff); // + Client version accepted - please send Game-ID to join
    printf("S: %s", recvBuff);

    size = formatCommand(sendBuff, "ID", opt->gameId);
    if (send(sock, sendBuff, size, 0) > 0)
    {
        printf("C: %s", sendBuff);
    }
    else
    {
        exit(EXIT_FAILURE);
    }

    recvCommand(sock, recvBuff); // + PLAYING <<Gamekind-Name>>\n + <<Game-Name>>
    printf("S: %s", recvBuff);

    size = formatCommand(sendBuff, "PLAYER", opt->playerId);
    if (send(sock, sendBuff, size, 0) > 0)
    {
        printf("C: %s", sendBuff);
    }
    else
    {
        exit(EXIT_FAILURE);
    }

    recvCommand(sock, recvBuff); //
    printf("S: %s", recvBuff);
}