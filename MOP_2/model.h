#ifndef MODEL_H
#define MODEL_H

#define NUM_COLUMNS 7
#define NUM_FOUNDATIONS 4

typedef struct Card
{
    char rank;
    char suit;
    int visible;
    struct Card* next;
} Card;

typedef struct
{
    Card* columns[NUM_COLUMNS];
    Card* foundations[NUM_FOUNDATIONS];
} Game;

// TODO LOTS OF STUFF

#endif