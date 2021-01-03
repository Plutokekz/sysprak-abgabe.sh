#ifndef connectionUtil
#define connectionUtil
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

#include "config.h"
#include "performConnection.h"
#include "shareMemory.h"

#define GAME_ID_SIZE 13
#define PORTNUMBER 1357
#define HOSTNAME "sysprak.priv.lab.nm.ifi.lmu.de"
#define GAMEKINDNAME "Bashni"
#define CLIENT_VERSION "2.42"

typedef struct Opt {
  char gameId[14];
  char playerId[2];
} opt_t;

// Commands that should be sendable to the server
typedef enum COMMAND {
  VERSION,
  ID,
  PLAYER,
  OKWAIT,
  THINKING,
  MOVEOK,
  PLAY
} COMMAND;

void performConnection(int sock, opt_t *opt, config_t *config);

// Sends command from Enum with value string after to the server. Value can also
// be an empty string, but must not be a nullpointer (WIP)
void sendCommand(COMMAND c, char *value);

// Returns address of buffer with complete command sent by the server.
// Returns everything, including "+ " and newline characters
// Very rarely breaks, depending on how server sends commands (WIP)
char *recvCommand();

#endif