//
// Created by ttorr on 30-04-2026.
//

#ifndef MOP_2_TYPES_H
#define MOP_2_TYPES_H

typedef struct Card // uses for linked lists
{
    char rank;
    char suit;
    int visible; // (0,1)
    struct Card* next; // reference to the next card in the linked list
} Card;

typedef struct Foundation {
    Card* ref; // reference to the first card in the linked list
    char suit;
} Foundation;

typedef struct Column
{
    Card* ref; // reference to the first card in the linked list
} Column;

#endif //MOP_2_TYPES_H