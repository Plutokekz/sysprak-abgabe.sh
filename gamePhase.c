#include "gamePhase.h"
#include <sys/epoll.h>

int epfd;
struct epoll_event *events;

void setupEpoll(int fd[2]){
  events = malloc(sizeof(struct epoll_event));
  epfd = epoll_create(1);
  events[0].events = EPOLLIN; // Cann append "|EPOLLOUT" for write events as well
  events[0].data.fd = fd[0];
  epoll_ctl(epfd, EPOLL_CTL_ADD, fd[0], &events[0]);

}

void gamePhase(int sock, int fd[2], int shmId) {
  char *buffer = "";
  void *shmPtr;
  //int num_ready;
  setupEpoll(fd);


  shmPtr = attachSHM(shmId);
  int size = 0;

  while (strncmp(buffer, "+ GAMEOVER", 10) != 0) {
    buffer = recvCommand(sock, 1, &size);
    printf("Current command: %s", buffer);

    if (strncmp(buffer, "+ WAIT", 6) == 0) {
      free(buffer);
      sendCommand(sock, OKWAIT, "");
    }

    printf("1\n");

    if (strncmp(buffer, "+ MOVE", 6) == 0) {
      printf("2\n");
      sendCommand(sock, THINKING, "");
      printf("3\n");
      buffer = recvCommand(sock, 26, &size);
      printf("4\n");
      printf("Piece List:\n%s", buffer+16);

      memcpy(shmPtr + sizeof(Share), buffer, size - 12);

      kill(getppid(), SIGUSR1);

      buffer = recvCommand(sock, 1, &size); // OKTHINK
      char *move = NULL;
      int move_size = 0;

      epoll_wait(epfd, events, 1, 1000 /*timeout*/);
      if (events[0].events & EPOLLIN) {
        printf("Socket %d got some data\n", events[0].data.fd);
        receiveCMD(fd[0], &move, &move_size);
        printf("Received: %s\n", move);
      }

      printf("send move\n");

      sendCommand(sock, PLAY, move);
      //free(move);
      //free(events);
      printf("move send\n");
      buffer = recvCommand(sock, 1, &size); // OKTHINK
      printf("buffer after send move: %s\n", buffer);

      // free(buffer);
      exit(-100);
    }
    printf("end of move \n");

    if (strncmp(buffer, "+ QUIT", 6) == 0) {
      free(buffer);
      printf("Server closed connection during gamephase due to protocoll "
             "error. \n");
      exit(EXIT_FAILURE);
    }
  }
  free(buffer);

  // strcpy(shmPtr, buffer);

  // TODO Tim: Printing out last gamestate and who won
}