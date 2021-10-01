#include "3d_bitboard.h"
#include <stdio.h>
#include "log.h"

typedef unsigned long long layer;

#define WHITE_START_BOARD 11163050ULL

#define BLACK_START_BOARD 6172839697753047040ULL

int MAPPING[] = {7,6,5,4,3,2,1,0};


/** @brief prints the given layer
  *
  * crapy print funktion to print all bits
  * from a unsigned long long with coordinates.
  *
  */
void showbits(layer x){
    int i=0;
    printf(" +--------+\n8|");
    for (i = (sizeof(layer) * 8) - 1; i >= 0; i--){
        if((i+1) % 8 == 0 && i+1 < 64) printf("|\n%d|", (i+1)/8);
       putchar(x & (1ULL << i) ? '1' : '0');
    }
    printf("|\n");
    printf(" +--------+\n");
    printf("  abcdefgh\n");
}


/** @brief flips a bit
  *
  * flips the bit on the given position
  * of the layer (0 - 63), 64 bits.
  *
  *
  */
void flip_bit(layer *l, int position){
    *l = *l ^ (1ULL << position);
}

/** @brief Converting a string coord to x and y.
 *
 *    ABCDEFGH
 *   +--------+
 *  8|01010101|8
 *  7|10101010|7
 *  6|01010101|6
 *  5|00000000|5
 *  4|00000000|4
 *  3|00000000|3
 *  2|00000000|2
 *  1|00000000|1
 *   +--------+
 *    ABCDEFGH
 *
 * H8 -> x = 0; y = 7 -> (y*8)+x = 7*8+0 = 56
 *
 *  @return int number_of_left_shifts
 */
int conv_coord(char *c){
    if (*c < 'A' || *c > 'H'){
        log_error("first coordinate <%c> not in range of 'A-H'", *c);
        return -1;
    }
    int x = MAPPING[*c - 'A'];
    c++;
    if (*c < '1' || *c > '8'){
        log_error("second coordinate <%c> not in range od '1-8'", *c);
        return -1;
    }
    int y = *c - '1';
    log_debug("coord %s, X: %d, Y: %d", --c, x, y);
    return (y*8)+x;

}

int main(){
    layer one = 0;
    showbits(one);
    char * c_0 = "H8";
    char * c_1 = "A8";
    char * c_2 = "A1";
    char * c_3 = "H1";

    int s = conv_coord(c_0);
    flip_bit(&one, s);

    s = conv_coord(c_1);
    flip_bit(&one, s);
    s = conv_coord(c_2);
    flip_bit(&one, s);
    s = conv_coord(c_3);
    flip_bit(&one, s);
    showbits(one);
    return 0;
}
