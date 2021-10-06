#include "parse_piecelist.h"


void printf_position(position_t *position){
    printf("Position{color: %c, position: %s}\n", position->color, position->position);
}

void free_position(position_t *position){
    if (position != NULL){
        free(position);
    }
}

position_t *_parse_piece(char *piece){
    position_t *position = malloc(sizeof(position_t));
    piece += 2; // skipp "|+ |"
    position->color = *piece;
    piece += 2; // skipp "|b@|"
    memccpy(position->position, piece, '\n', sizeof(char)*2);
    //printf_position(position);
    return position;
}

position_t **parse_pieclist(char *pieclist){
    pieclist += 16;
    position_t **positions = malloc(sizeof(position_t *)*24);
    int i;
    for (i=0;i<24;i++){
        positions[i] = _parse_piece(pieclist);
        pieclist += 7;
    }
    return positions;
}

/*
int main(){
    char *pieclist = "+ PIECESLIST 24\n+ b@H8\n+ b@F8\n+ b@G7\n+ b@E7\n+ b@H6\n+ b@F6\n+ w@E5\n+ w@E5\n+ w@E5\n+ w@E5\n+ b@E5\n+ w@H4\n+ w@H4\n+ w@H4\n+ w@H4\n+ w@H4\n+ w@H4\n+ b@H4\n+ B@H4\n+ w@F4\n+ b@F4\n+ B@F4\n+ b@F4\n+ w@G3\n+ ENDPIECESLIST";
    printf("%s\n", pieclist);
    position_t **p = parse_pieclist(pieclist);
    int i;
    for (i=0;i<24;i++){
        printf_position(p[i]);
        free_position(p[i]);
    }
    free(p);
    return 0;
}
*/
