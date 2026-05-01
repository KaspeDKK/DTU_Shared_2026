#ifndef MODEL_H
#define MODEL_H

#include "../Controller/CardServices.h"
#include "../Model/Types.h"

#define NUM_COLUMNS 7
#define NUM_FOUNDATIONS 4


void create_game(Card *deckHead, Column cols[]);

Card *getNth(Card *deckHead, int n);




// TODO LOTS OF STUFF

#endif