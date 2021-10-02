#include "bitboard.h"

int MAPPING[] = {7, 6, 5, 4, 3, 2, 1, 0};

void showbits(bitboard *board){
    int i=0;
    for (i = (sizeof(board) * 8) - 1; i >= 0; i--){
        if((i+1) % 8 == 0 && i+1 < 64) printf("\n");
        putchar(*board & (1ULL << i) ? '1' : '0');
    }
    printf("\n");
}

void _flip_bit(bitboard *board, int shift){
    *board = *board ^ (1ULL << shift);
}

bool _get_bit(bitboard *board, int shift){
    return *board & (1ULL << shift);
}

int coord_to_shift(int x, int y){
    if (x>=0&&x<=7&&y>=0&&y<=7){
        return (MAPPING[y]*8)+MAPPING[x];
    }
    log_error("Coordinates x: %d, y: %d not in range of 0-7");
    return -1;
}

void flip_bit(bitboard *board, int x, int y){
    int shift = coord_to_shift(x, y);
    if (shift != -1){
        _flip_bit(board, shift);
    }
}

bool get_bit(bitboard *board, int x, int y){
    int shift = coord_to_shift(x, y);
    if (shift != -1){
        return _get_bit(board, shift);
    }
    return NULL;
}
