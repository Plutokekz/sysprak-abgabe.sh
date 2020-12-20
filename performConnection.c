#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "performConnection.h"

#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define GAMEKINDNAME "Bashni"
#define CLIENT_VERSION "2.42"
#define BUFF_SIZE 100

void printServerResponse(char recvBuff[BUFF_SIZE], char *command) {
  char outBuff[BUFF_SIZE];

  if (command == NULL) {
    printf("S: %s", recvBuff);
  } 
  //else if (strcmp(command, "PLAYER") == 0) {
  //  int playerId;
  //  int total;
  //  char playerName[BUFF_SIZE];
  //  sscanf(recvBuff, "+ YOU %i %[^\n]\n+ TOTAL %i", &playerId, playerName,
  //         &total);
  //  printf("playerId: %i\n", playerId);
  //  printf("total: %i\n", total);
  //  printf("playerName: %s\n", playerName);
  //
  //  printf("S: + YOU ARE NOW %s")
  //}
}

void recvCommand(int sock, char buff[BUFF_SIZE]) {

  ssize_t bytes = 0;

  do {
    ssize_t tmp;
    switch ((tmp = recv(sock, buff + bytes, (BUFF_SIZE - bytes) - 1, 0))) {
    case -1:
      perror("receive error");
      exit(EXIT_FAILURE);
    case 0:
      printf("connection closed by server\n");
      exit(EXIT_FAILURE);

    default:
      bytes += tmp; // increment returned bytes every loop (at least 1 byte)
      break;
    }
  } while (buff[bytes - 1] != '\n' && BUFF_SIZE > bytes);

  buff[bytes] = '\0';

  if (buff[0] == '-') {
    printServerResponse(buff, NULL);
    exit(EXIT_FAILURE);
  }
}

// return size for s
size_t formatCommand(char buff[BUFF_SIZE], char *command, char *value) {
  memset(buff, 0, BUFF_SIZE);
  int rv;

  if (strlen(value) == 0) {
    rv = sprintf(buff, "%s\n", command);
  } else {
    rv = sprintf(buff, "%s %s\n", command, value);
  }

  if (rv > 0)
    return rv;
  printf("command formatting error\n");
  exit(EXIT_FAILURE);
}

void performConnection(int sock, gameOpt *opt) {
  in_addr_t addr;
  struct sockaddr_in server;
  struct hostent *host;

  if ((host = gethostbyname(HOSTNAME)) ==
      NULL) // upgrade to better function later
  {
    printf("can't resolve hostname\n");
    exit(EXIT_FAILURE);
  }

  memcpy(&server.sin_addr, host->h_addr_list[0], sizeof(addr));

  server.sin_family = AF_INET; // upgrade to IPv6 later
  server.sin_port = htons(PORTNUMBER);

  if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("connection failed");
    exit(EXIT_FAILURE);
  }

  char recvBuff[BUFF_SIZE];
  char sendBuff[BUFF_SIZE];
  size_t size;

  recvCommand(sock, recvBuff); // + MNM Gameserver <<Gameserver Version>>
                               // accepting connections
  printServerResponse(recvBuff, NULL);

  size = formatCommand(sendBuff, "VERSION", CLIENT_VERSION);
  if (send(sock, sendBuff, size, 0) > 0) {
    printf("C: %s", sendBuff);
  } else {
    exit(EXIT_FAILURE);
  }

  recvCommand(
      sock,
      recvBuff); // + Client version accepted - please send Game-ID to join
  printServerResponse(recvBuff, NULL);

  size = formatCommand(sendBuff, "ID", opt->gameId);
  if (send(sock, sendBuff, size, 0) > 0) {
    printf("C: %s", sendBuff);
  } else {
    exit(EXIT_FAILURE);
  }

  recvCommand(sock, recvBuff); // + PLAYING <<Gamekind-Name>>\n + <<Game-Name>>
  printServerResponse(recvBuff, NULL);

  size = formatCommand(sendBuff, "PLAYER", opt->playerId);
  if (send(sock, sendBuff, size, 0) > 0) {
    printf("C: %s", sendBuff);
  } else {
    exit(EXIT_FAILURE);
  }

  recvCommand(sock, recvBuff); //
  printServerResponse(recvBuff, NULL);
}