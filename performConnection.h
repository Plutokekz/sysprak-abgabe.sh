#ifndef connectionUtil
#define connectionUtil

#define GAME_ID_SIZE 13

typedef struct Opt
{
    char gameId[14];
    char playerId[2];
} opt_t;

typedef enum COMMAND { VERSION, ID, PLAYER } COMMAND;

void performConnection(int sock, opt_t *opt);

void sendCommand(COMMAND c, char *value);

char *recvCommand();

#endif