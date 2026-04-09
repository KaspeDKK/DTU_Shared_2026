// card.h
#ifndef CARD_H
#define CARD_H

typedef struct Card {
    int rank;
    char suit;
    int face_up; //0 hvis gemt, 1 hvis den skal vise.
    struct Card *next;
} Card;

#endif