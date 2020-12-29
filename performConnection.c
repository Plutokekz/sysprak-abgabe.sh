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

int SOCK;

void printServerResponse(char *recvBuff, char *command) {
  //char outBuff[BUFF_SIZE];

  if (command == NULL) {
    printf("S: %s", recvBuff);
  }
  // else if (strcmp(command, "PLAYER") == 0) {
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

char *recvCommand() {
  ssize_t bytes = 0;
  const size_t BUFF_SIZE = 50;
  int count = 2;
  char *buff = malloc(count * BUFF_SIZE);

  do {
    ssize_t tmp;
    switch ((tmp = recv(SOCK, buff + bytes, BUFF_SIZE - 1, 0))) {
    case -1:
      perror("receive error");
      free(buff);
      exit(EXIT_FAILURE);
    case 0:
      printf("connection closed by server\n");
      printf("already received: %s\n", buff);
      free(buff);
      exit(EXIT_FAILURE);
    default:
      bytes += tmp; // increment returned bytes every loop (at least 1 byte)
      if ((count * BUFF_SIZE) - bytes < BUFF_SIZE) {
        buff = realloc(buff, ++count * BUFF_SIZE); // no error handling
      }
    }
  } while (buff[bytes - 1] != '\n');

  buff[bytes] = '\0';

  if (buff[0] == '-') {
    printServerResponse(buff, NULL);
    exit(EXIT_FAILURE);
  }
  return buff;
}

char *lookup(COMMAND c) {
  char *command;
  switch (c) {
  case VERSION:
    command = "VERSION";
    break;
  case ID:
    command = "ID";
    break;
  case PLAYER:
    command = "PLAYER";
    break;
  default:
    printf("lookup COMMAND failed\n");
    exit(EXIT_FAILURE);
  }
  return command;
}

// supports VERSION, ID, PLAYER
void sendCommand(COMMAND c, char *value) {
  char sendBuff[50] = "";
  char *command = lookup(c);

  size_t size;
  if (strlen(value) == 0) {
    size = sprintf(sendBuff, "%s\n", command);
  } else {
    size = sprintf(sendBuff, "%s %s\n", command, value);
  }

  if (send(SOCK, sendBuff, size, 0) > 0) {
    printf("C: %s", sendBuff);
  } else {
    printf("sending failed\n");
    exit(EXIT_FAILURE);
  }
}

void setupConnection() {
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

  if (connect(SOCK, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("connection failed");
    exit(EXIT_FAILURE);
  }
}

void performConnection(int sock, opt_t *opt) {
  SOCK = sock;
  setupConnection(sock);

  char *recvBuff;

  recvBuff = recvCommand(); // + MNM Gameserver <<Gameserver Version>>
                            // accepting connections
  printServerResponse(recvBuff, NULL);

  sendCommand(VERSION, CLIENT_VERSION);

  recvBuff =
      recvCommand(); // + Client version accepted - please send Game-ID to join
  printServerResponse(recvBuff, NULL);

  sendCommand(ID, opt->gameId);

  recvBuff = recvCommand(); // + PLAYING <<Gamekind-Name>>\n + <<Game-Name>>
  printServerResponse(recvBuff, NULL);

  // retry without player id later
  sendCommand(PLAYER, opt->playerId);

  recvBuff = recvCommand(); //
  printServerResponse(recvBuff, NULL);

  free(recvBuff);
}