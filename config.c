//
// Created by Lukas on 15.12.2020.
//
#include "config.h"

void printfConfig(config_t *config) {
  printf("<Config{\n   game: %s\n   host: %s\n   port: %d\n}>\n", config->game,
         config->host, config->port);
}

void freeConfig(config_t *config) {
  if (config != NULL) {
    if (config->game != NULL) {
      free(config->game);
    }
    if (config->host != NULL) {
      free(config->host);
    }
    free(config);
  }
}

int parseAttr(char *attr, char *value, config_t *config) {
  if (strcmp(attr, "game") == 0) {
    config->game = value;
  } else if (strcmp(attr, "host") == 0) {
    config->host = value;
  } else if (strcmp(attr, "port") == 0) {
    config->port = atoi(value);
    free(value);
  } else {
    printf("Error the attribute <%s> is not available in the Config Struct and "
           "value <%s>\n",
           attr, value);
    free(attr);
    free(value);
    return -1;
  }
  free(attr);
  return 0;
}

char *parseWord(char *ptr, char c, int len) {
  if (ptr == NULL) {
    ptr = (char *)malloc(sizeof(char));
  } else {
    ptr = (char *)realloc(ptr, sizeof(char) * len + 1);
  }
  ptr[len] = c;
  return ptr;
}

int parseLine(char *line, config_t *config) {
  char *attr = NULL, *value = NULL, current;
  int index = 0, is_value = 0, length_attr = 0, length_value = 0;

  while ((current = line[index]) != '\0') {
    index++;
    switch (current) {
    case ' ':
      continue;
    case '=':
      is_value = 1;
      continue;
    case '\n':
      continue;
    default:
      if (is_value == 0) {
        attr = parseWord(attr, current, length_attr);
        length_attr++;
      } else {
        value= parseWord(value, current, length_value);
        length_value++;
      }
    }
  }
  if (attr != NULL || value != NULL) {
    //printf("length attr: %d, length value: %d\n", length_attr, length_value);
    attr = (char *)realloc(attr, length_attr +1);
    attr[length_attr] = '\0';

    value = (char *)realloc(value, length_value + 1);
    value[length_value] = '\0';

    //printf("Attribute: %s with the Value: %s\n", attr, value);
    return parseAttr(attr, value, config);
  }

  return -1;

}

config_t *readConfigFile(char *filename) {
  FILE *fp;
  config_t *config = malloc(sizeof(config_t));
  char *str = malloc(sizeof(char) * MAX_LINE_LENGTH);

  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Could not open file %s", filename);
    return NULL;
  }

  while (fgets(str, MAX_LINE_LENGTH, fp) != NULL) {
    if (parseLine(str, config) != 0) {
      freeConfig(config);
      free(str);
      fclose(fp);
      return NULL;
    }
  }
  free(str);
  fclose(fp);

  return config;
}
