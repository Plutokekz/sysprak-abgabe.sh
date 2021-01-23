//
// Created by Lukas on 04.01.2021.
//

#include "cmdPipe.h"

int sendCMD(int fdw, char *cmd) {

  int n = strnlen(cmd, MAXCOMMANDLENGTH) + 1;

  if (write(fdw, &n, sizeof(int)) < 0) {
    perror("Error writing size to pipe");
    return -1;
  }

  if (write(fdw, cmd, sizeof(char) * n) < 0) {
    perror("Error writing cmd to pipe");
    return -1;
  }
  return 0;
}

int receiveCMD(int fdr, char **cmd, int *size) {
  int n;
  if (read(fdr, &n, sizeof(int)) < 0) {
    perror("Error reading cmd size from pipe");
    return -1;
  }

  if (*cmd == NULL) {
    *size = n;
    if ((*cmd = (char *)malloc(sizeof(char) * n)) == NULL) {
      perror("Error malloc cmd string");
      return -1;
    }
  } else {
    if (*size < n) {
      *size = n;
      if ((*cmd = (char *)realloc(*cmd, sizeof(char) * n)) == NULL) {
        perror("Error realloc cmd string");
        return -1;
      }
    }
  }

  if (read(fdr, *cmd, sizeof(char) * n) < 0) {
    perror("Error reading cmd from pipe");
    return -1;
  }

  return 0;
}