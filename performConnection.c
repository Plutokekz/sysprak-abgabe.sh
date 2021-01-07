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
char commandBuff[RECV_BUFF_SIZE];

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

int charCount(char *s, int n) {
  int count = 0;
  for (int i = 0; i < n; i++) {
    count += (s[i] == '\n');
  }
  return count;
}

void *recvCommand(int lines) {
  int lineCount = lines;
  if (lines == 0) {
    lineCount = INT_MAX;
  }

  ssize_t bytes = 0;
  int count = 2;
  char *buff = malloc(count * RECV_BUFF_SIZE);
  strcpy(buff, commandBuff);
  bytes += strlen(commandBuff);

  while (lineCount > 0) {
    ssize_t tmp;
    switch ((tmp = recv(SOCK, buff + bytes, RECV_BUFF_SIZE - 1, 0))) {
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
      if (buff[0] == '-') {
        buff[bytes] = '\0';
        printf("%s", buff);
        free(buff);
        exit(EXIT_FAILURE);
      }
      lineCount -= charCount(buff + bytes, tmp);
      bytes += tmp; // increment returned bytes every loop (at least 1 byte)
      buff[bytes] = '\0';
      char *end;
      if (lineCount > lines && (end = strstr(buff, "END")) != NULL) {
        end = strchr(end, '\n');
        memset(commandBuff, 0, RECV_BUFF_SIZE);
        strcpy(commandBuff,
               end + 1); // splitted string size is less than RECV_BUFF_SIZe
        end[1] = '\0';
        lineCount = 0;
      } else if ((count * RECV_BUFF_SIZE) - bytes < RECV_BUFF_SIZE) {
        buff = realloc(buff, ++count * RECV_BUFF_SIZE); // no error handling
      }
      break;
    }
  }
  return (void *)buff;
}

// supports VERSION, ID, PLAYER
void sendCommand(COMMAND c, char *value) {
  char sendBuff[50] = "";
  char *command = lookup(c);
  size_t size;
  if (value == NULL || strlen(value) == 0) {
    size = sprintf(sendBuff, "%s\n", command);
  } else {
    size = sprintf(sendBuff, "%s %s\n", command, value);
  }
  if (send(SOCK, sendBuff, size, 0) < 0) {
    perror("sending failed\n");
    exit(EXIT_FAILURE);
  }
}

int _connect(char *hostname, int port) {

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

  if (connect(SOCK, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("connection failed");
    return -1;
  }
  return 0;
}

int setupConnectionByStruct(config_t *config) {
  if (_connect(config->host, config->port) != 0) {
    return -1;
  }
  return 0;
}

int setupConnection() {
  if (_connect(HOSTNAME, PORTNUMBER) != 0) {
    return -1;
  }
  return 0;
}

void performConnection(int sock, opt_t *opt, config_t *config) {

  SOCK = sock;

  if (config == NULL) {
    printf("Using Default Settings\n");
    if (setupConnection() != 0) {
      exit(EXIT_FAILURE);
    }
  } else {
    printf("Using Config Settings\n");
    printfConfig(config);
    if (setupConnectionByStruct(config) != 0) {
      freeConfig(config);
      exit(EXIT_FAILURE);
    }
    freeConfig(config);
  }

  void *recvBuff;
  recvBuff = recvCommand(1); // + MNM Gameserver <<Gameserver Version>>
                             // accepting connections
  parseCommand(recvBuff, START);
  free(recvBuff);

  sendCommand(VERSION, CLIENT_VERSION);

  recvBuff =
      recvCommand(1); // + Client version accepted - please send Game-ID to join
  parseCommand(recvBuff, VERSION);
  free(recvBuff);

  sendCommand(ID, opt->gameId);

  recvBuff = recvCommand(2); // + PLAYING <<Gamekind-Name>>\n + <<Game-Name>>
  parseCommand(recvBuff, ID);
  free(recvBuff);

  // retry without player id later
  sendCommand(PLAYER, opt->playerId);

  recvBuff = recvCommand(0); //
  parseCommand(recvBuff, PLAYER);
  free(recvBuff);

  // SHM (TODO Tim: Einbinden, sobald Erik's struct erstellt wurde)
  // int shmID = setupSHM_GameStart(struct x);
  // shmID über Pipeline an Parent Prozess (Thinker) schicken

  game_info *gameInfo = getGameInfo();
  printProlog(gameInfo, PRETTY);
  freeGameInfo(gameInfo);
}