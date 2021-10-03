//
// Created by Lukas on 16.12.2020.
//

#include "../modules/utils.h"
#include <stdlib.h>
int main() {
  char *validFile = "config.conf";
  config_t *vconf = malloc(sizeof(config_t)),*invconf = malloc(sizeof(config_t));

  int r = readConfigFile(validFile, vconf);
  if (r != -1) {
    printfConfig(vconf);
    freeConfig(vconf);
  }

  char *invalidFile = "config_false.conf";
  r = readConfigFile(invalidFile, invconf);
  if (r != -1){
    printfConfig(invconf);
    free(invconf);
  }
}
