#include "gamePhase.h"
#include <sys/epoll.h>
#include <time.h>

int epfd;
struct epoll_event *events;

void setupEpoll(int fd[2]) {
  printf("gamePhase pipe fd[0]: %d, fd[1]: %d\n", fd[0], fd[1]);
  events = malloc(sizeof(struct epoll_event));
  //initialisierung
  events->events = 0;
  events->data.fd = 0;
  events->data.ptr = NULL;
  events->data.u32 = 0;
  events->data.u64 = 0;
  //
  epfd = epoll_create(1);
  events[0].events = EPOLLIN;
  events[0].data.fd = fd[0];
  epoll_ctl(epfd, EPOLL_CTL_ADD, fd[0], &events[0]);
}

void gamePhase(int sock, int fd[2], int shmId) {
  char *buffer;
  struct Share *shmPtr;
  setupEpoll(fd);
  shmPtr = attachSHM(shmId);
  int size = 0;
  char *move = NULL;
  int move_size = 0;
  // main game loop receiving commands, thinking and sending them back
  while (strncmp(buffer, "+ GAMEOVER", 10) != 0) {
    buffer = recvCommand(sock, 1, &size); // receive WAIT or MOVE command
    // handle wait command
    if (strncmp(buffer, "+ WAIT", 6) == 0) {
      free(buffer);
      sendCommand(sock, OKWAIT, "");
    }
    // handle move commands
    if (strncmp(buffer, "+ MOVE", 6) == 0) {
      sendCommand(sock, THINKING, "");
      buffer = recvCommand(sock, 26, &size);
      // write piece list buffer to shared memmory
      memcpy((void *)shmPtr + sizeof(Share), buffer, size - 12);
      shmPtr->thinkerGuard = 1;
      // send signal to thinker
      kill(getppid(), SIGUSR1);
      // receive OKWAIT command
      buffer = recvCommand(sock, 1, &size); // OKTHINK
      if (strncmp(buffer, "+ OKTHINK", 9) == 0) {
        // wait for thinker written move to pipe
        epoll_wait(epfd, events, 1, 1000 /*timeout*/);
        if (events[0].events & EPOLLIN) {
          receiveCMD(events[0].data.fd, &move, &move_size);
        }
        sendCommand(sock, PLAY, move);
        // receive MOVEOK command
        buffer = recvCommand(sock, 1, &size); // OKTHINK
        printf("buffer after send move: %s\n", buffer);
      }
    }
    if (strncmp(buffer, "+ QUIT", 6) == 0) {
      free(move);
      free(shmPtr); // mybe special free ?
      free(buffer);
      printf("Server closed connection during gamephase due to protocoll "
             "error. \n");
      exit(EXIT_FAILURE);
    }
  }
  free(buffer);
  buffer = recvCommand(sock, 26, &size); //receive piecelist
  char *pieceList;
  pieceList = malloc(300);
  memcpy(pieceList, buffer, 300);
  bitboard_t *currentBoard = parsFromString(pieceList);
  printBitboard(currentBoard);
  
  free(buffer);
  buffer = recvCommand(sock, 1, &size);
  if (strncmp(buffer, "+ PLAYER0WON Yes", 16) == 0) {
    printf("White won! \n");
  }
  free(buffer);
  buffer = recvCommand(sock, 1, &size);
  if (strncmp(buffer, "+ PLAYER1WON Yes", 16) == 0) {
    printf("Black won! \n");
  }

  free(buffer);
  free(move);
  free(shmPtr); // mybe special free ?
  free(buffer);
  exit(EXIT_SUCCESS);
}
