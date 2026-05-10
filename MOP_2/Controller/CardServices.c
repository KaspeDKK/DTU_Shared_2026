//
// Created by kaspe on 30-04-2026.
//
#include <stdlib.h>
#include "CardServices.h"

#include <stdio.h>

void placeCard(Column *column, Card *card) {
    struct Card *newNode = (struct Card *)malloc(sizeof(struct Card));
    newNode->rank = card->rank;
    newNode->suit = card->suit;
    newNode->visible = card->visible;
    newNode->next = NULL;

    // If the column is empty, set it as the head
    if (column->ref == NULL) {
        column->ref = newNode;
        return;
    }

    // Traverse to the bottom of the column
    Card *current = column->ref;
    while (current->next != NULL) {
        current = current->next;
    }

    // Append at the bottom
    current->next = newNode;

}

// Parse a char string into a card struct
Card parseCard(const char *cardStr) {
    Card card;
    card.rank = cardStr[0];
    card.suit = cardStr[1];
    card.visible = 1;  // Cards being moved should be visible
    card.next = NULL;
    return card;
}

int moveCard(Card *moveCard, Column *columnFrom, Column *columnTo) {
    // Validate that no pointers are NULL
    if (moveCard == NULL || columnFrom == NULL || columnTo == NULL) {
        printf("Illegal move\n");
        return 0;
    }

    // Hidden cards cannot be moved
    if (!moveCard->visible) {
        printf("Cannot move hidden card\n");
        return 0;
    }

    Card* headCard = columnFrom->ref;
    Card* endOfColumn = getLastCard(*columnTo);

    // if the destination column is empty, we can only place a king (rank 13)
    if (endOfColumn == NULL) {
        if (determineRank(*moveCard) != 13) {
            printf("Illegal move\n");
            return 0;
        }

        // Check if the card is the first in the fromColumn
        if (headCard->rank == moveCard->rank && headCard->suit == moveCard->suit) {
            columnFrom->ref = NULL;
            columnTo->ref = moveCard;
            return 1;
        }

        // Find the first card in the fromColumn
        while (headCard->next != NULL &&
               (headCard->next->rank != moveCard->rank ||
                headCard->next->suit != moveCard->suit)) {
            headCard = headCard->next;
                }

        if (headCard->next == NULL) {
            printf("Picked card is not in column\n");
            return 0;
        }

        // Move card and make the new top card of the fromColumn visible
        columnTo->ref = headCard->next;
        headCard->next = NULL;

        if (columnFrom->ref != NULL) {
            getLastCard(*columnFrom)->visible = 1;
        }

        return 1;
    }

    // Dette burde fikse problem med at flytte sidste kort i kolonnen
    if (headCard->rank == moveCard->rank && headCard->suit == moveCard->suit) {
        // The card is the first in column -> update pointer from columnFrom to null
        if (columnFrom->ref == moveCard) {
            columnFrom->ref = NULL;
            if (isMoveLegal(moveCard, endOfColumn)== 1) {
                endOfColumn->next = moveCard;
                return 1;
            } else {
                printf("Illegal move\n");
                columnFrom->ref = moveCard; // Restore on failure
                return 0;
            }
        }
        // if the card is not the first, skip and continue
        columnFrom->ref = moveCard->next;
        if (isMoveLegal(moveCard, endOfColumn)== 1) {
            endOfColumn->next = moveCard;
            return 1;
        } else {
            printf("Illegal move\n");
            columnFrom->ref = headCard; // Restore on failure
            return 0;
        }
    }
    //iterate through the column to find the card to move, and keep track of the card before it (headCard)
    while (headCard->next != NULL && (headCard->next->rank != moveCard->rank || headCard->next->suit != moveCard->suit)) {
        headCard = headCard->next;
    }


    if (headCard->next == NULL) {
        printf("Picked card is not in column\n");
        return 0;
    }
    printf("MOVE: %c%c -> %c%c\n",
       moveCard->rank, moveCard->suit,
       endOfColumn->rank, endOfColumn->suit);

    if (isMoveLegal(moveCard, endOfColumn)== 1) { //condition check
        Card* cardToMove = headCard->next;
        headCard->next = NULL;
        endOfColumn->next = cardToMove;

        //If the moved card was the head of the column, set old column pointer to null
        if (columnFrom->ref == moveCard) {
            columnFrom->ref = NULL;
        }

        // last card of each column is always visible
        if (columnFrom->ref != NULL) getLastCard(*columnFrom)->visible = 1;
        if (columnTo->ref != NULL) getLastCard(*columnTo)->visible = 1;
        return 1;
    } else {
        printf("Illegal move\n");
        return 0;
    }
}

// Returns a pointer to the last card in a column
Card* getLastCard(Column column) {
    if (column.ref == NULL){ return NULL;}
    while (column.ref->next != NULL) {
        column.ref = column.ref->next;
    }
    return column.ref;
}

// Returns a pointer to the last card in a foundation
Card* getLastCardFoundation(Foundation foundation) {
    if (foundation.ref == NULL){ return NULL;}
    while (foundation.ref->next != NULL) {
        foundation.ref = foundation.ref->next;
    }
    return foundation.ref;
}

int moveCardFoundation(Card *moveCard, Column *columnFrom, Foundation *foundation) {
    Card* endOfFoundation = getLastCardFoundation(*foundation);
    Card* endOfColumn = getLastCard(*columnFrom);

    // Only the top card in the column can be moved to foundation// Kun sidste kort i kolonnen må rykkes til foundation
    if (endOfColumn->rank != moveCard->rank || endOfColumn->suit != moveCard->suit) {
        printf("Illegal move\n");
        return 0;
    }

    Card* headCard = columnFrom->ref;
    while (headCard->next != NULL && (headCard->next->rank != moveCard->rank || headCard->next->suit != moveCard->suit)) {
        headCard = headCard->next;
    }

    // The first card placed on a founcation must be an ace (rank 1)
    if (endOfFoundation == NULL) {
        if (determineRank(*moveCard) != 1) {
            printf("Illegal move\n");
            return 0;
        }
        //Edge case: If the card is the first in the column, set columnFrom to NULL
        foundation->ref = moveCard;
        if (columnFrom->ref == moveCard) {
            columnFrom->ref = NULL;
        } else {
            headCard->next = NULL; // Remove card from column
        }
        moveCard->next = NULL;

        // Make the new top card of the column visible
        Card *newLast = getLastCard(*columnFrom);
        if (newLast != NULL) {
            newLast->visible = 1;
        }

        return 1;
    }

    if (headCard->next == NULL && headCard != moveCard) {
        printf("Card not found in column\n");
        return 0;
    }

    // Column contains only one card
    if (columnFrom->ref->rank == moveCard->rank && columnFrom->ref->suit == moveCard->suit ) {
        if (isMoveLegalFoundation(moveCard, endOfFoundation) != 1) {
            printf("Illegal move\n");
            return 0;
        }
        columnFrom->ref = NULL;
        moveCard->next = NULL;
        endOfFoundation->next = moveCard;

        Card *newLast = getLastCard(*columnFrom);
        if (newLast != NULL) {
            newLast->visible = 1;
        }
        return 1;

    }

    // Move the card from the column to the foundation
    if (isMoveLegalFoundation(moveCard, endOfFoundation) == 1) {
        Card* cardToMove = headCard->next;
        headCard->next = NULL;
        endOfFoundation->next = cardToMove;
        return 1;
    } else {
        printf("Illegal move\n");
        return 0;
    }
}

int moveCardFromFoundation(Card *moveCard, Column *columnTo, Foundation *fromFoundation) {
    Card* endOfFoundation = getLastCardFoundation(*fromFoundation);
    Card* endOFColumn = getLastCard(*columnTo);

    // Cant move to empty column from foundation
    if (columnTo->ref==NULL) {
        printf("Illegal move\n");
        return 0;
    }
    if (isMoveLegal(endOfFoundation, endOFColumn)) {
        // Move card to column
        Card* cardToMove = endOfFoundation;
        endOFColumn->next = cardToMove;

        // Remove the card from foundation
        Card* current = fromFoundation->ref;
        while (current->next != endOfFoundation && current->next != NULL) {
            current = current->next;
        }
        // If foundation contains only one card
        if (fromFoundation->ref == moveCard) {
            fromFoundation->ref = NULL;
        }

        current->next = NULL;
        return 1;
    }
    return 0;
}

// Converts a card's rank character to numeric value
int determineRank(Card card) {
    switch (card.rank) {
        case 'A': return 1;
        case 'T': return 10;
        case 'J': return 11;
        case 'Q': return 12;
        case 'K': return 13;
        default:
            if (card.rank < '2' || card.rank > '9') return -1;
            return card.rank - '0';
    }
}

// Checks if a move is legal
int isMoveLegal(Card* moveCard, Card* cardTo) {
    int cardRank = determineRank(*moveCard);
    int cardRank2 = determineRank(*cardTo);

    if (moveCard->suit == cardTo->suit) {return 0;}
    if (cardRank != cardRank2-1) {return 0;}

    return 1;
}

// Check if a move to the foundation is legal
int isMoveLegalFoundation(Card* moveCard, Card* cardTo) {
    int cardRank = determineRank(*moveCard);
    int cardRank2 = determineRank(*cardTo);

    if (moveCard->suit != cardTo->suit) {return 0;}
    if (cardRank != cardRank2+1) {return 0;}

    return 1;
}

// Makes sure the top card of every column is always visible
void showAllLastCards(Column cols[]) {
    //Brute force
    Card* lastCard;
    for (int i = 0; i < NUM_COLUMNS; i++) {
        lastCard = getLastCard(cols[i]);
        if (lastCard == NULL) {
            continue;
        }
            lastCard->visible = 1;
        }

    }
