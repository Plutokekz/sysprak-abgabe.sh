#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "protocolUtil.h"

#define RECV_BUFF_SIZE 100

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

// supports VERSION, ID, PLAYER
void sendCommand(int sock, COMMAND c, char *value) {
  char sendBuff[50] = "";
  char *command = lookup(c);
  size_t size;
  if (value == NULL || strlen(value) == 0) {
    size = sprintf(sendBuff, "%s\n", command);
  } else {
    size = sprintf(sendBuff, "%s %s\n", command, value);
  }
  if (send(sock, sendBuff, size, 0) < 0) {
    perror("sending failed\n");
    exit(EXIT_FAILURE);
  }
}

void *recvCommand(int sock, int lines) {
  static char buffer[RECV_BUFF_SIZE];
  ssize_t totalBytes = 0; // number of bytes in buff
  int blockCount = 2;
  char *recvBuff = malloc(
      blockCount * RECV_BUFF_SIZE); // initializing 2 memory blocks, so if one
                                    // fills up the next one can just be used
  // fills up recvBuff with oddments from last call
  strcpy(recvBuff, buffer);
  totalBytes += strlen(buffer);

  int lineCount = lines == 0 ? INT_MAX : lines;
  while (lineCount > 0) {
    ssize_t receivedBytes;
    switch ((receivedBytes =
                 recv(sock, recvBuff + totalBytes, RECV_BUFF_SIZE - 1, 0))) {
    case -1:
      perror("receive error");
      free(recvBuff);
      exit(EXIT_FAILURE);
    case 0:
      printf("connection closed by server\n");
      printf("already received: %s\n", recvBuff);
      free(recvBuff);
      exit(EXIT_FAILURE);
    default:
      if (recvBuff[0] == '-') {
        recvBuff[totalBytes] = '\0';
        printf("%s", recvBuff);
        free(recvBuff);
        exit(EXIT_FAILURE);
      }
      // lineCount -= linenCount(recvBuff + totalBytes, receivedBytes);
      int bytes = 0;
      while (bytes < receivedBytes) {
        if (recvBuff[totalBytes + bytes] == '\n') {
          lineCount--;
        }
        bytes++;
        if (lineCount <= 0)
          break;
      }
      if (bytes < receivedBytes) {
        recvBuff[totalBytes + receivedBytes] = '\0';
        strcpy(buffer, recvBuff + totalBytes + bytes);
      }
      totalBytes +=
          bytes; // increment returned bytes every loop (at least 1 byte)
      recvBuff[totalBytes] = '\0';
      char *end;
      if (lineCount > lines && (end = strstr(recvBuff, "END")) != NULL) {
        end = strchr(end, '\n');
        memset(buffer, 0, RECV_BUFF_SIZE);
        strcpy(buffer,
               end + 1); // splitted string size is less than RECV_BUFF_SIZe
        end[1] = '\0';
        lineCount = 0;
      } else if ((blockCount * RECV_BUFF_SIZE) - totalBytes < RECV_BUFF_SIZE) {
        recvBuff = realloc(recvBuff,
                           ++blockCount * RECV_BUFF_SIZE); // no error handling
      }
      break;
    }
  }
  return (void *)recvBuff;
}