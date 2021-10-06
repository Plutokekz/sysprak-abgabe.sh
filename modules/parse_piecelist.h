/** @file parse_piecelist.h
  *
  * @brief Contains everything to parse the string piecelist
  *
  * Parses the string piecelist to an array of positions
  *
  * print, free, parse
  *
  * @author Lukas Mahr
  *
  * @date 06.10.2021
  *
  * @bug not known
  *
  */
#ifndef PARSE_PIECELIST
#define PARSE_PIECELIST
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** @brief Position struct
  *
  *
  */
typedef struct Position{
    char color;          /**< color char which contains the color of the piece at the position#color. */
    char position[2];    /**< position contrains the position at the grid p.e. H8#position[2] */
}position_t;

/** @brief prints the position struct
  *
  * @param *position a pointer to position struct you want to print
  *
  */
void printf_position(position_t *position);

/** @brief free a position
  *
  * @param *position the position you want to free
  *
  * frees the given position pointer
  *
  */
void free_position(position_t *position);

/** @brief Parse a Piece
  *
  * @param *char to parse piece from, example "+ b@H8\n"
  *
  * @return position_t* pointer to the parsed position
  *
  */
position_t *_parse_piece(char *piece);

/** @brief Parses piecelist to an array of position_t's
  *
  * @param *piecelist a complete piecelist starting with "+ PIECELIST 24\n ...\n+ ENDPICELIST"
  *
  * parses line by line from the given piecelist and add the position_t to an array of position_t's
  *
  * @return **positions an array of the positions from the piecelist.
  *
  */
position_t **parse_pieclist(char *pieclist);

#endif
