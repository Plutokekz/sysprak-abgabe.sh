//
// Created by Lukas on 15.12.2020.
//

#ifndef BASCHNI_CONFIG_H
#define BASCHNI_CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 300

typedef struct Config {
    int port;
    char *host;
    char *game;
} config_t;

config_t *readConfigFile(char *filename);

int parseLine(char *line, config_t *config);

int parseAttr(char *attr, char *value, config_t *config);

void freeConfig(config_t *config);

void printfConfig(config_t *config);

int parseAttr(char *attr, char *value, config_t *config);

char *parseWord(char *ptr, char c, int len);

#endif //BASCHNI_CONFIG_H
