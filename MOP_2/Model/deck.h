//
// Created by ttorr on 04-05-2026.
//

#ifndef MOP_2_DECK_H
#define MOP_2_DECK_H
#include "Types.h"

Card *arrayToList(Card deck[], int size);
Card* readDeck(const char *filename, Card *deck);
Card *splitDeck (Card *head, int split);
void listToArray(Card *head, Card deck[], int size);

Card *randomShuffle(Card *head);

#endif //MOP_2_DECK_H