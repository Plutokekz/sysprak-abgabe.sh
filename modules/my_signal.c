#include "signal.h"
#include "../thinker.h"


void thinking() { thinker(); }

void activateSignal () {
    struct sigaction sa = {0};
    sa.sa_handler = &thinking;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, NULL);
}
