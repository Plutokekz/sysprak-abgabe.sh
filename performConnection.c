#include <arpa/inet.h>
#include <errno.h>
#include <limits.h>
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
#define RECV_BUFF_SIZE 100

int SOCK;

// prefix nullable
void rawAppend(game_info *gameInfo, char *recvBuff) {
  static char *prolBuff = NULL;
  static size_t size = 0;
  size_t len = strlen(recvBuff);
  if (size == 0) {
    prolBuff = malloc(len + 1);
    strcpy(prolBuff, recvBuff);
  } else {
    prolBuff = realloc(prolBuff, size + len + 1);
    strcat(prolBuff + size, recvBuff);
  }
  size += len;
  gameInfo->raw = prolBuff;
}

// parse and frees
// expects null terminated string
game_info *parseCommand(char *recvBuff, COMMAND c) {
  static game_info gameInfo;
  // static player_t **playerList = NULL;
  if (recvBuff == NULL || c == RETURN) {
    return &gameInfo;
  }
  rawAppend(&gameInfo, recvBuff);
  int rv = 0;
  switch (c) {
  case START: {
    char *serverVersionBuff = calloc(strlen(recvBuff), 1);
    size_t maxServerVersionSize = sizeof(gameInfo.serverVersion);
    if ((rv = sscanf(recvBuff, "%*[^0-9]%s", serverVersionBuff)) > 0 &&
        strlen(serverVersionBuff) < maxServerVersionSize) {
      strcpy(gameInfo.serverVersion, serverVersionBuff);
    }
    free(serverVersionBuff);
  } break;
  case VERSION:
    break;
  case ID: {
    size_t maxGamekindNameSize = sizeof(gameInfo.gamekindName);
    size_t maxGameNameSize = sizeof(gameInfo.gameName);
    char *gamekindNameBuff = calloc(strlen(recvBuff), 1);
    char *gameNameBuff = calloc(strlen(recvBuff), 1);
    if ((rv = sscanf(recvBuff, "+ PLAYING %[^\n]\n+ %[^\n]", gamekindNameBuff,
                     gameNameBuff)) > 0 &&
        strlen(gamekindNameBuff) < maxGamekindNameSize &&
        strlen(gameNameBuff) < maxGameNameSize) {
      strcpy(gameInfo.gamekindName, gamekindNameBuff);
      strcpy(gameInfo.gameName, gameNameBuff);
    }
    free(gamekindNameBuff);
    free(gameNameBuff);
  } break;
  case PLAYER: {
    int n;
    player_t player;
    player.ready = 1;
    size_t maxPlayerNameSize = sizeof(player.playerName);
    char *playerNameBuff = calloc(strlen(recvBuff), 1);
    char *rest = calloc(strlen(recvBuff), 1);
    if ((rv = sscanf(recvBuff, "+ YOU %u %[^\n]\n+ TOTAL %u\n%n",
                     &player.playerId, playerNameBuff, &gameInfo.total, &n)) >
            0 &&
        strlen(playerNameBuff) < maxPlayerNameSize) {
      strcpy(player.playerName, playerNameBuff);
      // own player initialized, continue with player list
      gameInfo.playerList = malloc(gameInfo.total * sizeof(player_t *));
      gameInfo.playerList[0] = malloc(sizeof(player_t));
      memcpy(gameInfo.playerList[0], &player, sizeof(player_t));
      for (unsigned int i = 1; i < gameInfo.total; i++) {
        gameInfo.playerList[i] = malloc(sizeof(player_t));
        int tmpN;
        if ((rv = sscanf(recvBuff + n, "+ %u %[^\n]\n%n", &player.playerId,
                         playerNameBuff, &tmpN)) > 0) {
          size_t playerNameSize = strlen(playerNameBuff);
          if (playerNameSize < maxPlayerNameSize) {
            // playerName = <<Player-Name>> <<Ready>>
            playerNameBuff[playerNameSize - 2] = '\0';
            player.ready =
                playerNameBuff[playerNameSize - 1] == '1' ? true : false;
            strcpy(player.playerName, playerNameBuff);
            memcpy(gameInfo.playerList[i], &player, sizeof(player_t));
          }
        }
        n += tmpN;
      }
    }
    free(playerNameBuff);
    free(rest);
    break;
  }
  default:
    printf("command not supported\n");
    exit(EXIT_FAILURE);
    break;
  }
  if (rv == -1) {
    perror("parsing commands failed\n");
    exit(EXIT_FAILURE);
  }
  return &gameInfo;
}

// helper
game_info *getGameInfo() { return parseCommand(NULL, RETURN); }

void freeGameInfo(game_info *gameInfo) {
  if (gameInfo != NULL) {
    free(gameInfo->raw);
    for (unsigned int i = 0; i < gameInfo->total; i++) {
      free(gameInfo->playerList[i]);
    }
    free(gameInfo->playerList);
  }
}

void printProlog(game_info *gameInfo, P_FLAG f) {
  switch (f) {
  case PRETTY:
    printf("Your are connected with MNM Gameserver v%s\n",
           gameInfo->serverVersion);
    printf("In %s %s:\n", gameInfo->gamekindName, gameInfo->gameName);
    for (unsigned int i = 0; i < gameInfo->total; i++) {
      if (gameInfo->playerList[i]->ready) {
        printf("Player %u (%s) is ready\n",
               gameInfo->playerList[i]->playerId + 1,
               gameInfo->playerList[i]->playerName);
      } else {
        printf("Player %u (%s) is not ready\n",
               gameInfo->playerList[i]->playerId + 1,
               gameInfo->playerList[i]->playerName);
      }
    }
    break;
  case DEBUG:
    printf("%s", gameInfo->raw);
    break;
  default:
    printf("wrong flag\n");
    exit(EXIT_FAILURE);
    break;
  }
}

int _connect(int sock, char *hostname, int port) {

  in_addr_t addr;
  struct sockaddr_in server;
  struct hostent *host;

  // upgrade to better function later
  if ((host = gethostbyname(hostname)) == NULL) {
    printf("can't resolve hostname\n");
    return -1;
  }

  memcpy(&server.sin_addr, host->h_addr_list[0], sizeof(addr));

  server.sin_family = AF_INET; // upgrade to IPv6 later
  server.sin_port = htons(port);

  if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("connection failed");
    return -1;
  }
  return 0;
}

int setupConnectionByStruct(int sock, config_t *config) {
  if (_connect(sock, config->host, config->port) != 0) {
    return -1;
  }
  return 0;
}

int setupConnection(int sock) {
  if (_connect(sock, HOSTNAME, PORTNUMBER) != 0) {
    return -1;
  }
  return 0;
}

void performConnection(int sock, config_t *config) {
  int size = 0;
  if (config->game == NULL || config->host == NULL || config->port == 0) {
    printf("Using Default Settings\n");
    if (setupConnection(sock) != 0) {
      freeConfig(config);
      exit(EXIT_FAILURE);
    }
  } else {
    printf("Using Config Settings\n");
    printfConfig(config);
    if (setupConnectionByStruct(sock, config) != 0) {
      freeConfig(config);
      exit(EXIT_FAILURE);
    }
    freeConfig(config);
  }

  void *recvBuff;
  recvBuff = recvCommand(sock, 1, &size); // + MNM Gameserver <<Gameserver
                                          // Version>> accepting connections
  parseCommand(recvBuff, START);
  free(recvBuff);

  sendCommand(sock, VERSION, CLIENT_VERSION);

  recvBuff = recvCommand(
      sock, 1,
      &size); // + Client version accepted - please send Game-ID to join
  parseCommand(recvBuff, VERSION);
  free(recvBuff);

  sendCommand(sock, ID, config->gameId);

  recvBuff = recvCommand(
      sock, 2, &size); // + PLAYING <<Gamekind-Name>>\n + <<Game-Name>>
  parseCommand(recvBuff, ID);
  free(recvBuff);

  // retry without player id later
  sendCommand(sock, PLAYER, config->playerId);

  recvBuff = recvCommand(sock, 0, &size); //
  parseCommand(recvBuff, PLAYER);
  free(recvBuff);

  printProlog(getGameInfo(), config->f);
}
