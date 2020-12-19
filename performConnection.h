#ifndef connectionUtil
#define connectionUtil

#define GAME_ID_SIZE 13

typedef struct gameOpt
{
    char gameId[14];
    char playerId[2];
} gameOpt;


void performConnection(int sock, gameOpt *opt);

#endif