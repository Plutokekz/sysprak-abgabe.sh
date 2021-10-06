/** @file game_phase.h
  *
  * @brief contains everythink for the game phase
  *
  * For communication between thinker and connector this method might need the
  * pipe ID and thinker process ID for signal. For testing purposes this has not
  * been implemented yet, but should be expanded later.
  *
  *
  * @bug not known
  */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "modules/cmd_pipe.h"
#include "modules/share_memory.h"
#include "modules/protocol_utils.h"
#include "signal.h"
#include "modules/old_bitboard.h"
#include "modules/log.h"

void gamePhase(int sock, int fd[2], int shmId);
