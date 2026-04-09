#include <stdio.h>
#include <stdlib.h>
#include "card.h"
#define DECKSIZE 52;

int main(void) { //game is played here:
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\n");
    printf("[]\t[]\t[]\t[]\t[]\t[]\t[]\n");
    return 0;
}

Card *LD(const char *filename){ //deckOne
    FILE *file = fopen(filename,"r");//read "deckOne"

    Card card;


}