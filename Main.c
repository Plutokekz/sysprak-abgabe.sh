#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "performConnection.h"

int main(int argc, char *argv[]) {

  int sock;
  int c;
  opt_t opt = { "", "" };

  while ((c = getopt(argc, argv, "g:p:")) != -1) {
    switch (c) {
    case 'g':
      if (strlen(optarg) != GAME_ID_SIZE) {
        printf("wrong game id\n");
        exit(EXIT_FAILURE);
      }
      strcpy(opt.gameId, optarg);
      break;
    case 'p': {
      unsigned int id;
      if (sscanf(optarg, "%ui", &id) < 1 || !(id == 1 || id == 2)) {
        printf("wrong player id\n");
        exit(EXIT_FAILURE);
      }
      strcpy(opt.playerId, optarg);
      break;
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
  if (strlen(opt.gameId) == 0) {
    printf("game id is missing\n");
    exit(EXIT_FAILURE);
  }

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("creating socket failed");
    return EXIT_FAILURE;
  }

  performConnection(sock, &opt);

  close(sock);

  return EXIT_SUCCESS;
}
