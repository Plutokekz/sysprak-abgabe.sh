#ifndef BASCHNI_THINKER_H
#define BASCHNI_THINKER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#include "modules/bitboard.h"
#include "modules/log.h"
#include "modules/cmdPipe.h"
#include "modules/shareMemory.h"

void initThinker(int *fd);

void thinker();

#endif
