//
// Created by ttorr on 04-05-2026.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "deck.h"

Card* readDeck (const char *filename,Card *deck) { //function that takes a file, and
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        file = fopen("../deckOne.txt", "r");
        if (file == NULL) {
            perror("Error opening file");
            return NULL;
        }
    }

    int i = 0;
    char buffer[10];

    while (i < 52 && fgets(buffer, sizeof(buffer), file) != NULL) {
        if (buffer[0] == '\n' || buffer[0] == '\0') {
            continue;
        }

        deck[i].rank = buffer[0];
        deck[i].suit = buffer[1];
        deck[i].visible = 0;
        i++;
    }

    fclose(file);
    return arrayToList(deck, i);
}

Card *splitDeck (Card *head, int split) { //splits deck
    if (head == NULL) { //guard
        printf("LAST Command SI");
        printf("Message: no deck loaded\n");
        return NULL;
    }
    if (split == 0) { //Generate random split, 0 < split > 51
        split = rand() % 51 + 1;
    }
    if (split < 1 || split > 51) {
        printf("LAST Command SI");
        printf("Message: Wrong split Size\n");
        return NULL;
    }

    Card *oldPile = head; //initiate oldPile
    Card *current = head;

    //guard if the pile is big enough
    for (int i = 1; i < split; i++) {
        if (current == NULL) { //if current is null:
            return head;
        }
        current = current->next; //else "increment"
    }

    if (current == NULL || current->next == NULL) {
        return head;
    }

    Card *newPile = current->next; //Start newPile from current
    current->next = NULL;

    //initiate placeholders for the next cards
    Card *nextOldCard = NULL;
    Card *nextNewCard = NULL;

    //while neither is null, continue to mix cards between each other
    while (oldPile != NULL && newPile != NULL) {
        nextOldCard = oldPile->next;
        nextNewCard = newPile->next;

        oldPile->next = newPile;

        if (nextOldCard == NULL) {
            break;
        }

        newPile->next = nextOldCard; //set the cards back together

        //increment
        oldPile = nextOldCard;
        newPile = nextNewCard;
    }

    return head; //return new pile
}

Card *randomShuffle(Card *head) {
    srand(time(NULL)); //generates a seed from the current time, for generating a random number.
    int sizeNewDeck = 1;
    Card *current = NULL;
    Card *shuffledDeck = head;

    if (head == NULL) return NULL;

    head = head->next;
    shuffledDeck->next = NULL;

    while (head != NULL) {
        Card *insertCard = head;
        head = head->next;
        insertCard->next = NULL;

        Card *placeholder = shuffledDeck;

        int randomIndex = rand() % sizeNewDeck +1;


        if (randomIndex == 0) {
            insertCard->next = shuffledDeck;
            shuffledDeck = insertCard;
        } else {
            current = placeholder;
            for (int i = 1; i < randomIndex; i++) {
                current = current->next;
            }

            insertCard->next = current->next;
            current->next = insertCard;
        }

        sizeNewDeck = sizeNewDeck + 1;
    }

    return shuffledDeck;
}



void listToArray(Card *head, Card deck[], int size)
{
    Card *current = head;

    for (int i = 0; i < size && current != NULL; i++)
    {
        deck[i].rank = current->rank;
        deck[i].suit = current->suit;
        deck[i].visible = current->visible;

        current = current->next;
    }
}