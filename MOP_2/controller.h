#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "model.h"

void run_game();

Card* readDeck(const char *filename, Card *deck);
void listToArray(Card *head, Card deck[], int size);
Card *arrayToList(Card deck[], int size);

#endif