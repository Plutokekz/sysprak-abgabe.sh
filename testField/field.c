#include "field.h"

#define MAX_TOWERSIZE 12
#define MAX_TOWERS 12

char StringToStructArray (char *plString) {
    pieceStruct field[32];
    char *pos;
    char piece;
    char xPosChar;
    int xPos;
    int yPos;
    int doneGuard = 0;
    while (fgets(pos, MAX_TOWERSIZE,plString) != NULL) {
        piece = pos[2];// {w,W,b,B}
        xPosChar = pos[4]; //{A-H}
        switch (xPosChar) {
            case 'A':
                xPos = 1;
                break;
            case 'B':
                xPos = 2;
                break;
            case 'C':
                xPos = 3;
                break;
            case 'D':
                xPos = 4;
                break;
            case 'E':
                xPos = 5;
                break;
            case 'F':
                xPos = 6;
                break;
            case 'G':
                xPos = 7;
                break;
            case 'H':
                xPos = 8;
                break;
        }
        
        yPos = pos[5] - '0'; //{1-8}
        if (field )

    }
    return plArray;
}



int main () { //zum Testen

    char testS[] = "w@A1\nb@A1\nb@A1\nw@C1\n";
    printf("testS length: %d\n", strlen(testS));



    return 0;
}





        /*for (int i = 0;i <= MAX_TOWERS;i++) {
            if (plArray[i][0] == xPos) {
                
                if (plArray[i][0] == yPos) { //Piece gehört auf ext Turm
                    strncat(plArray[i], piece, sizeof(char));
                    doneGuard = 1;
                }
                
            }
            

        }
        if (!doneGuard) {
            for (int i = 0;i <= MAX_TOWERS;i++) {
                if (plArray[i] == NULL) {
                    plArray[i] = xPos + yPos + ": " + piece; //umformen
                }
            }
        }*/