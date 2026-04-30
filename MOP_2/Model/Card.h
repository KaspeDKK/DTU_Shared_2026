//
// Created by kaspe on 30-04-2026.
//



#ifndef MOP_2_CARD_H
#define MOP_2_CARD_H

typedef struct Card
{
    char rank;
    char suit;
    int visible; // (0,1)
    struct Card* next;
} Card;

#endif //MOP_2_CARD_H

