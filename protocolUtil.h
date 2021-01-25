/**
 * @brief COMMAND enum
 * \enum COMMAND
 *
 * Contains all relevant commands for following functions.
 */
typedef enum COMMAND { RETURN, START, VERSION, ID, PLAYER, THINKING, OKWAIT, PLAY } COMMAND;

/** @brief Sends command to server.
 *
 * The function parses and concatenates the command with the value
 * passed to the function and afterwards send it to the server.
 *
 *  @param sock socket file descriptor
 *  @param c enum COMMAND
 *  @param value value that will be appended to command
 */
void sendCommand(int sock, COMMAND c, char *value);

/** @brief Receives server response after sending a command.
 *
 * The function receives the number of lines passed as parameter.
 * If lines is 0, then the function waits until an END command
 * (e. g. ENDPLAYERS or ENDPIECELIST) is received. It returns
 * a pointer to a dynamically allocated memory that must be freed.
 *
 *  @param sock socket file descriptor
 *  @param lines expected lines the server will send
 *  @return pointer to received data
 */
void *recvCommand(int sock, int lines, int *size);