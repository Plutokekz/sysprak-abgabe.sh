//
// Created by Lukas on 05.01.2021.
//

#include "../modules/cmdPipe.h"

int main() {
  pid_t pid;

  int fd[2]; //, n = 5;
  // char string[] = "abc\n\n";

  if (pipe(fd) < 0) {
    perror("Error creating pipe");
    exit(EXIT_FAILURE);
  }

  // splitting up in thinker and connector
  if ((pid = fork()) < 0) {
    perror("fork failed");
    return EXIT_FAILURE;
  } else if (pid == 0) {
    // child process connector

    close(fd[1]);

    char *msg = NULL;
    int size = 0;

    receiveCMD(fd[0], &msg, &size);

    printf("received message: %s, with size: %i\n", msg, size);

    receiveCMD(fd[0], &msg, &size);

    printf("received message: %s, with size: %i\n", msg, size);

    free(msg);

  }
  else {
    // parent process thinker

    close(fd[0]);

    char *cmd = "A3:B4";

    sendCMD(fd[1], cmd);

    cmd = "B6:A5";

    sendCMD(fd[1], cmd);

    // last part of thinker; executed after game over
    if (waitpid(pid, NULL, 0) == -1) {
      perror("error waiting for connector");
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
