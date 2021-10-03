/** @file thinker.h
  *
  * @brief Contains the thinker
  *
  *
  * just the think funktion and its modules
  *
  * @bugs not known
  *
  */
#ifndef BASCHNI_THINKER_H
#define BASCHNI_THINKER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#include "modules/old_bitboard.h"
#include "modules/log.h"
#include "modules/cmdPipe.h"
#include "modules/shareMemory.h"

/** @brief initialises the Thinker
  *
  * @param *fd pipe for the thinker
  *
  * closes one side of the pipe end writes to the other side
  * if he finished thinking
  *
  */
void initThinker(int *fd);

/** @brief thinks
  *
  * calculates the next move and send's over the pipe to @gamePhase.h
  *
  */
void thinker();

#endif
