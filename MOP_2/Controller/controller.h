#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../Model/model.h"

void game_startup();
void run_game(Card *deckHead);
Card* readDeck(const char *filename, Card *deck);
Card *splitDeck (Card *head, int split);
void listToArray(Card *head, Card deck[], int size);
Card *arrayToList(Card deck[], int size);
Card *randomShuffle(Card *head);
Card* getLastCard(Column *column);
Card* getLastCardFoundation(Foundation *foundation);
int isMoveLegal(Card* moveCard, Card* cardTo);
int isMoveLegalFoundation(Card* moveCard, Card* cardTo);
Card parseCard(const char *cardStr);
void moveCardFoundation(Card *moveCard, Column *columnFrom, Foundation *foundation);
void moveCard(Card *moveCard, Column *columnFrom, Column *columnTo);


#endif