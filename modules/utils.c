//
// Created by Lukas on 15.12.2020.
//
#include "utils.h"

void printfConfig(config_t *config) {
  if (config != NULL) {
    printf("<Config{game: %s, host: %s, port: %d}>\n", config->game,
           config->host, config->port);
  } else {
    printf("Given Config is NULL\n");
  }
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
  config = NULL;
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
        value = parseWord(value, current, length_value);
        length_value++;
      }
    }
  }
  if (attr != NULL || value != NULL) {
    printf("length attr: %d, length value: %d\n", length_attr, length_value);
    attr = (char *)realloc(attr, length_attr + 1);
    attr[length_attr] = '\0';

    value = (char *)realloc(value, length_value + 1);
    value[length_value] = '\0';

    printf("Attribute: %s with the Value: %s\n", attr, value);
    return parseAttr(attr, value, config);
  }

  return -1;
}

int parseLine_f(char *line, config_t *config, int line_index) {

  char current, *attribute, *value;
  int is_value = 0, length_attr = 0, length_value = 0, index = 0,
      start_index_value = 0, start_index_attr = 0, a = 0, v = 0;

  while ((current = line[index]) != '\0') {

    if (current == '=') {
      is_value = 1;
    }

    else if ((current >= 'A' && current <= 'Z') ||
             (current >= 'a' && current <= 'z') || current == '.' ||
             (current >= '0' && current <= '9')) {
      if (is_value != 1) {
        length_attr++;
        if (a == 0) {
          start_index_attr = index;
          a = 1;
        }
      } else {
        length_value++;
        if (v == 0) {
          start_index_value = index;
          v = 1;
        }
      }
    }
    index++;
  }

  if (is_value == 0) {
    printf("Config File formatting error in line <%i>\n", line_index);
    return -1;
  }

  attribute = (char *)malloc(sizeof(char) * length_attr + 1);
  attribute = memcpy(attribute, line + start_index_attr, length_attr);
  attribute[length_attr] = '\0';

  value = (char *)malloc(sizeof(char) * length_value + 1);
  value = memcpy(value, line + start_index_value, length_value);
  value[length_value] = '\0';

  // printf("Attribute %s, Value %s\n", attribute, value);

  return parseAttr(attribute, value, config);
}

int readConfigFile(char *filename, config_t *config) {
  FILE *fp;
  //config_t *config = malloc(sizeof(config_t));
  char *str = NULL;
  size_t line_size;
  int index = 0;

  fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Could not find <%s>\n", filename);
    printf("Using Default Config File\n");
    fp = fopen(DEFAULT_CONFIG_FILENAME, "r");
    if (fp == NULL) {
      printf("Default config file not available\n");
      return -1;
    }
  }

  while (getline(&str, &line_size, fp) != -1) {
    if (parseLine_f(str, config, index) != 0) {
      freeConfig(config);
      free(str);
      fclose(fp);
      return -1;
    }
    index++;
  }
  free(str);
  fclose(fp);

  return 0;
}

config_t *setOptions(int argc, char *argv[]) {
  config_t *config = malloc(sizeof(config_t));
  int c;
  config->f = PRETTY;
  while ((c = getopt(argc, argv, "g:p:c:d")) != -1) {
    switch (c) {
    case 'g':
      if (strlen(optarg) != GAME_ID_SIZE) {
        printf("wrong game id\n");
        exit(EXIT_FAILURE);
      }
      strcpy(config->gameId, optarg);
      break;
    case 'p': {
      unsigned int id;
      if (sscanf(optarg, "%u", &id) < 1 || id < 1 || id > 2) {
        printf("wrong player id\n");
        exit(EXIT_FAILURE);
      }
      snprintf(config->playerId, 2, "%u", id - 1);
      break;
    }
    case 'c':
      readConfigFile(optarg, config);
      break;
    case 'd':
      config->f = DEBUG;
      break;
    case '?':
      printf("wrong argument\n");
      freeConfig(config);
      exit(EXIT_FAILURE);
      break;

    default:
      break;
    }
  }
  // temporary add game id in Makefile
  if (strlen(config->gameId) == 0) {
    printf("game id is missing\n");
    exit(EXIT_FAILURE);
  }
  return config;
}
