#include "../modules/bitboard.h"

int main(){
    bitboard board = 0;

    showbits(&board);
    printf("\n");

    flip_bit(&board, 0, 0);
    printf("x: %d, y: %d\n", 0, 0);
    showbits(&board);
    printf("\n");
    flip_bit(&board, 0, 7);
    printf("x: %d, y: %d\n", 0, 7);
    showbits(&board);
    printf("\n");
    flip_bit(&board, 7, 0);
    printf("x: %d, y: %d\n", 7, 0);
    showbits(&board);
    printf("\n");
    flip_bit(&board, 7, 7);
    printf("x: %d, y: %d\n", 7, 7);
    showbits(&board);
    printf("\n");
    return 0;
}
