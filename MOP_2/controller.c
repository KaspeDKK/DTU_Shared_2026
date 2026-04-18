#include <stdio.h>
#include <string.h>
#include "controller.h"

#include <ctype.h>
#include <stdlib.h>

#include "model.h"
#include "view.h"

void run_game()
{
    Card deck[52] = {}; // deck size 52
    // TODO LOTS OF STUFF
    Card *deckHead = readDeck("../deckOne.txt", deck);


    showDeck(deckHead);

    deckHead = splitDeck(deckHead,0);

    showDeck(deckHead);

}

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

Card *splitDeck (Card *head, int split) {
    if (head == NULL) {
        printf("LAST Command SI");
        printf("Message: no deck loaded\n");
        return NULL;
    }
    if (split == 0) {
        split = rand() % 51 + 1;
    }
    if (split < 1 || split > 51) {
        printf("LAST Command SI");
        printf("Message: Wrong split Size\n");
        return NULL;
    }

    Card *oldPile = head;
    Card *current = head;

    for (int i = 1; i < split; i++) {
        if (current == NULL) {
            return head;
        }
        current = current->next;
    }

    if (current == NULL || current->next == NULL) {
        return head;
    }

    Card *newPile = current->next;
    current->next = NULL;

    Card *nextOldCard = NULL;
    Card *nextNewCard = NULL;

    while (oldPile != NULL && newPile != NULL) {
        nextOldCard = oldPile->next;
        nextNewCard = newPile->next;

        oldPile->next = newPile;

        if (nextOldCard == NULL) {
            break;
        }

        newPile->next = nextOldCard;

        oldPile = nextOldCard;
        newPile = nextNewCard;
    }

    return head;
}



Card *arrayToList(Card deck[], int size)
{
    Card *head = NULL;
    Card *current = NULL;

    for (int i = 0; i < size; i++)
    {
        Card *newNode = malloc(sizeof(Card));

        newNode->rank = deck[i].rank;
        newNode->suit = deck[i].suit;
        newNode->visible = deck[i].visible;
        newNode->next = NULL;

        if (head == NULL)
        {
            head = newNode;
            current = newNode;
        }
        else
        {
            current->next = newNode;
            current = newNode;
        }
    }

    return head;
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