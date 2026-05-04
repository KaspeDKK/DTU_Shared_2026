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

Card parseCard(const char *cardStr) {
    Card card;
    card.rank = cardStr[0];
    card.suit = cardStr[1];
    card.visible = 1;  // Cards being moved should be visible
    card.next = NULL;
    return card;
}

void moveCard(Card *moveCard, Column *columnFrom, Column *columnTo) {
    if (moveCard == NULL || columnFrom == NULL || columnTo == NULL) {
        printf("Illegal move\n");
        return;
    }

    Card* headCard = columnFrom->ref;
    Card* endOfColumn = getLastCard(*columnTo);

    // Dette burde fikse problem med at flytte sidste kort i kolonnen
    if (headCard->rank == moveCard->rank && headCard->suit == moveCard->suit) {
        //Hvis det kort der rykkes er det første kort, skal vi opdatere pointeren fra columnFrom til null
        if (columnFrom->ref == moveCard) {
            columnFrom->ref = NULL;
            if (isMoveLegal(moveCard, endOfColumn)== 1) {
                endOfColumn->next = moveCard;
                return;
            } else {
                printf("Illegal move\n");
                return;
            }
        }
        //Else
        columnFrom->ref = moveCard->next;
        if (isMoveLegal(moveCard, endOfColumn)== 1) {
            endOfColumn->next = moveCard;
            return;
        } else {
            printf("Illegal move\n");
            return;
        }
    }
    //iterate through the column to find the card to move, and keep track of the card before it (headCard)
    while (headCard->next != NULL && (headCard->next->rank != moveCard->rank || headCard->next->suit != moveCard->suit)) {
        headCard = headCard->next;
    }
    // if the column is empty
    if (endOfColumn == NULL) {

        // Kun konge kan placeres på en tom kolonne
        if (determineRank(*moveCard) != 13) {
            printf("Illegal move\n");
            return;
        }

        columnTo->ref = moveCard;
        //columnFrom->ref = moveCard->next;
        headCard->next = NULL;
        return;
    }

    if (headCard->next == NULL) {
        printf("Picked card is not in column\n");
        return;
    }
    printf("MOVE: %c%c -> %c%c\n",
       moveCard->rank, moveCard->suit,
       endOfColumn->rank, endOfColumn->suit);

    if (isMoveLegal(moveCard, endOfColumn)== 1) { //condition check
        Card* cardToMove = headCard->next;
        endOfColumn->next = cardToMove;

        //If the moved card was the head of the column, set old column pointer to null
        if (columnFrom->ref == moveCard) {
            columnFrom->ref = NULL;
        }

    } else {
        printf("Illegal move\n");
    }
}

Card* getLastCard(Column column) {
    if (column.ref == NULL){ return NULL;}
    while (column.ref->next != NULL) {
        column.ref = column.ref->next;
    }
    return column.ref;
}

Card* getLastCardFoundation(Foundation foundation) {
    if (foundation.ref == NULL){ return NULL;}
    while (foundation.ref->next != NULL) {
        foundation.ref = foundation.ref->next;
    }
    return foundation.ref;
}

void moveCardFoundation(Card *moveCard, Column *columnFrom, Foundation *foundation) {
    Card* endOfFoundation = getLastCardFoundation(*foundation);
    Card* endOfColumn = getLastCard(*columnFrom);

    // Kun sidste kort i kolonnen må rykkes til foundation
    if (endOfColumn->rank != moveCard->rank || endOfColumn->suit != moveCard->suit) {
        printf("Illegal move\n");
        return;
    }

    // Foundations første kort SKAL være ES (1)
    if (endOfFoundation == NULL) {
        if (determineRank(*moveCard) != 1) {
            printf("Illegal move\n");
            return;
        }
        foundation->ref = moveCard;
        columnFrom->ref = moveCard->next;
        moveCard->next = NULL;
        return;
    }

    // Kolonnen har kun ét kort
    if (columnFrom->ref->rank == moveCard->rank && columnFrom->ref->suit == moveCard->suit) {
        if (isMoveLegalFoundation(moveCard, endOfFoundation) != 1) {
            printf("Illegal move\n");
            return;
        }
        columnFrom->ref = NULL;
        moveCard->next = NULL;
        endOfFoundation->next = moveCard;
        return;
    }

    Card* headCard = columnFrom->ref;
    while (headCard->next != NULL && (headCard->next->rank != moveCard->rank || headCard->next->suit != moveCard->suit)) {
        headCard = headCard->next;
    }
    if (headCard->next == NULL) {
        printf("Card not found in column\n");
        return;
    }

    if (isMoveLegalFoundation(moveCard, endOfFoundation) == 1) {
        Card* cardToMove = headCard->next;
        headCard->next = NULL;
        endOfFoundation->next = cardToMove;
    } else {
        printf("Illegal move\n");
    }
}

void moveCardFromFoundation(Card *moveCard, Column *columnTo, Foundation *fromFoundation) {
    Card* endOfFoundation = getLastCardFoundation(*fromFoundation);
    Card* endOFColumn = getLastCard(*columnTo);

    if (isMoveLegal(endOfFoundation, endOFColumn)) {
        // flyt kortet til kolonnen
        Card* cardToMove = endOfFoundation;
        endOFColumn->next = cardToMove;

        // fjern kortet fra foundation
        Card* current = fromFoundation->ref;
        while (current->next != endOfFoundation) {
            current = current->next;
        }

        current->next = NULL;
    }
}

int determineRank(Card card) {
    switch (card.rank) {
        case 'A': return 1;
        case 'T': return 10;
        case 'J': return 11;
        case 'Q': return 12;
        case 'K': return 13;
        default:  return card.rank - '0'; // '2' -> 2, etc.
    }
}


int isMoveLegal(Card* moveCard, Card* cardTo) {
    int cardRank = determineRank(*moveCard);
    int cardRank2 = determineRank(*cardTo);

    if (moveCard->suit == cardTo->suit) {return 0;}
    if (cardRank != cardRank2-1) {return 0;}

    return 1;
}

int isMoveLegalFoundation(Card* moveCard, Card* cardTo) {
    int cardRank = determineRank(*moveCard);
    int cardRank2 = determineRank(*cardTo);

    if (moveCard->suit != cardTo->suit) {return 0;}
    if (cardRank != cardRank2+1) {return 0;}

    return 1;
}