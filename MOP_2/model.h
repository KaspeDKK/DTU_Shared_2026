#ifndef MODEL_H
#define MODEL_H

#define NUM_COLUMNS 7
#define NUM_FOUNDATIONS 4

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
};


// TODO LOTS OF STUFF

#endif