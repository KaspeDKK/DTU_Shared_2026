//
// Created by ttorr on 30-04-2026.
//

#ifndef MOP_2_TYPES_H
#define MOP_2_TYPES_H

typedef struct Card
{
    char rank;
    char suit;
    int visible; // (0,1)
    struct Card* next;
} Card;

typedef struct Foundation {
    Card* ref;
    char suit;
} Foundation;

typedef struct Column
{
    Card* ref;
} Column;




#endif //MOP_2_TYPES_H