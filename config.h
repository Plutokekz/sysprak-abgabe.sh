/** @file config.h
 *  @brief Function to read and pars the Config file.
 *
 *  This contains the functions for the Config
 *  file reader and parser and the config struct.
 *
 *  @author Lukas Lorenz Mahr (Plutokekz)
 *  @bug No known bugs.
 */

#ifndef BASCHNI_CONFIG_H
#define BASCHNI_CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_CONFIG_FILENAME "tests/config.conf"

/**
 * @brief Config struct
 * \struct Config
 *
 * Contains all the information that which should be red from the Config file.
 */
typedef struct Config {
  int port;   /**< the port number for the server as an integer#port. */
  char *host; /**< the ip address for the server as an pointer to a string#host.
               */
  char *game; /**< the name of the game for the server as an pointer to a
                 string#game. */
} config_t;

/** @brief reads the config file with the given filename.
 *
 * If the config file is corrupted or it contains an property which is not in
 * the parser list an Error gets thrown and a NULL pointer will be returned.
 *
 *  @param *filename the name of the config file
 *  @return config_t struct, NULL pointer if there was an Error.
 */
config_t *readConfigFile(char *filename);

/**
 * @brief frees a config_t struct
 * @param config pointer
 */
void freeConfig(config_t *config);

/**
 * @brief print a config_t struct
 * @param config pointer
 */
void printfConfig(config_t *config);

#endif // BASCHNI_CONFIG_H
