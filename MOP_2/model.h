#ifndef MODEL_H
#define MODEL_H

#define NUM_COLUMNS 7
#define NUM_FOUNDATIONS 4

void create_game(Card *deckHead);

Card getNth(Card *deckHead, int n);

typedef struct Card
{
    char rank;
    char suit;
    int visible; // (0,1)
    struct Card* next;
} Card;

typedef struct Column
{
    struct Card* ref;
} Column;


// TODO LOTS OF STUFF

#endif