
// For communication between thinker and connector this method might need the
// pipe ID and thinker process ID for signal. For testing purposes this has not
// been implemented yet, but should be expanded later.

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "modules/cmdPipe.h"
#include "modules/shareMemory.h"
#include "modules/protocolUtils.h"
#include "signal.h"
#include "modules/bitboard.h"
#include "modules/log.h"

void gamePhase(int sock, int fd[2], int shmId);
