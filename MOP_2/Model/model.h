#ifndef MODEL_H
#define MODEL_H

#include "../Controller/CardServices.h"
#include "../Model/Types.h"

#define NUM_COLUMNS 7
#define NUM_FOUNDATIONS 4

void create_game(Card *deckHead, Column cols[]); // Takes a linked list head for a deck of cards and assigns the cards into the columns in the classic yukon rowwise method. Additionally, sets the card visibilities.

#endif