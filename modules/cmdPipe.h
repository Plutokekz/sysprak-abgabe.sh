//
// Created by Lukas on 05.01.2021.
//
/** @file cmdPipe.h
 *  @brief transmits the moves from Thinker to Connector
 *
 *  This contains the functions to send end receive moves/commands
 *  over the given pipe.
 *
 *  The size of the command is currently looked by the strlen function MAX_COUNT
 *  which is set to MAXCOMMANDLENGTH
 *
 *  @author Lukas Lorenz Mahr (Plutokekz)
 *  @bug No known bugs.
 */
#ifndef BASCHNI_CMDPIPE_H
#define BASCHNI_CMDPIPE_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAXCOMMANDLENGTH 100

/** @brief send/writes the char array to the pipe
 *
 * calculates the length of the str, next the size + 1 (for the \0) gets writen
 * to the pipe. That the receiveCMD function knows the size of the writen
 * command.
 *
 *  @param fdw the file descriptor for thr write side.
 *  @param *cmd the command to be transmitted
 *  @return -1 if an error accrues else 0
 */
int sendCMD(int fdw, char *cmd);

/** @brief read/receives a char array from the pipe
 *
 * if the **cmd ist NULL it gets malloced in the size of the received command
 * from the pipe and the new size is writen to *size
 *
 * if the *size is smaller than the received command size **cmd gets realloced
 * and the new size gets writen to *size
 *
 *  @param fdr the file descriptor for thr read side.
 *  @param **cmd buffer for the received command.
 *  @param *size size of the buffer.
 *  @return -1 if an error accrues else 0
 */
int receiveCMD(int fdr, char **cmd, int *size);

int receiveInt(int fdr, int *value);

int sendInt(int fdw, int *value);

#endif // BASCHNI_CMDPIPE_H
