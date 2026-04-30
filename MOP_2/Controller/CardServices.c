//
// Created by kaspe on 30-04-2026.
//
#include <stdlib.h>
#include "CardServices.h"

void placeCard(Column column, Card *card) {
    struct Card *newNode = (struct Card *)malloc(sizeof(struct Card));
    newNode->rank = card->rank;
    newNode->suit = card->suit;
    newNode->next = NULL;

    // If the column is empty, set it as the head
    if (column.ref == NULL) {
        column.ref = newNode;
        return;
    }

    // Traverse to the bottom of the column
    Card *current = column.ref;
    while (current->next != NULL) {
        current = current->next;
    }

    // Append at the bottom
    current->next = newNode;

}