//
// Created by Lukas on 16.12.2020.
//

#include "../config.h"
#include <stdlib.h>
int main() {
  char *validFile = "config.conf";
  config_t *vconf = NULL,*invconf = NULL;

  vconf = readConfigFile(validFile);
  if (vconf != NULL) {
    printfConfig(vconf);
    freeConfig(vconf);
  }

  char *invalidFile = "config_false.conf";
  invconf = readConfigFile(invalidFile);
  if (invconf != NULL){
    printfConfig(invconf);
    free(invconf);
  }
}