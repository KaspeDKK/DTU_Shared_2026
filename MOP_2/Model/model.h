#ifndef MODEL_H
#define MODEL_H

#include "../Model/Card.h"

#define NUM_COLUMNS 7
#define NUM_FOUNDATIONS 4



typedef struct Column
{
    Card ref;
} Column;

void create_game(Card *deckHead);

struct Card *getNth(Card *deckHead, int n);




// TODO LOTS OF STUFF

#endif