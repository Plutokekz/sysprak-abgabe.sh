/**  @file bitboard.h
  *
  *  @brief Bitboard 8x8
  *
  * bitboard type, print, flipping
  *
  * @bug not known
  */
#ifndef BITBOARD_H
#define BITBOARD_H

#include <stdio.h>
#include <stdbool.h>

#include "log.h"

typedef unsigned long long bitboard;


/** @brief prints the given layer
  *
  * @param board bitboard pointer
  *
  * crapy print funktion to print all bits
  * from a unsigned long long with coordinates.
  *
  */
void showbits(bitboard *board);


/** @brief flips a bit
  *
  * @param board bitboard pointer
  *
  * flips the bit on the given position
  * of the layer (0 - 63), 64 bits.
  *
  */
void _flip_bit(bitboard *board, int position);

/** @brief flip the bit
  *
  * @param x int between 0-7
  * @param y int between 0-7
  *
  * flip the bit at the given x and y position.
  *
  */
void flip_bit(bitboard *board, int y, int x);

/** @brief get the bit
  *
  * @param board
  * @param x int between 0-7
  * @param y int between 0-7
  *
  * returns true or false if the bit at the given x and y position is 1 or 0
  *
  * @return bool
  *
  */
bool get_bit(bitboard *board, int x, int y);

#endif
