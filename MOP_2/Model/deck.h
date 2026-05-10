//
// Created by ttorr on 04-05-2026.
//

#ifndef MOP_2_DECK_H
#define MOP_2_DECK_H
#include "Types.h"

Card* readDeck(const char *filename, Card *deck); // function takes a filename and scans it. Generates a linked list from the file contents by using the deck pointer as head.
int saveDeck(const char *filename, Card *head); // saves a linked list to a given file
Card *splitDeck (Card *head, int split); // split shuffles a deck of cards using the parameter split as the value from which to split at.
void listToArray(Card *head, Card deck[], int size); // takes a deck of cards as a linked list and puts them into an array of cards.
Card *arrayToList(Card deck[], int size); // takes an array of cards and generates a linked list.
Card *randomShuffle(Card *head); // randomly shuffles a deck of cards.

#endif //MOP_2_DECK_H