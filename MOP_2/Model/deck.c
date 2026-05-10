#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "deck.h"
#include <string.h>

// Reads a deck from a file and returns it as a linked list
Card* readDeck (const char *filename,Card *deck) { //function that takes a file, and
    FILE *file = NULL;

    // if no filename was given, use default deck
    if (filename == NULL || strcmp(filename, "") == 0) {
        file = fopen("../default.txt", "r");
    }
    // if filename was given
    else {
        file = fopen(filename, "r");
    }
    if (file == NULL) {
        return NULL; // file was not found
    }

    int i = 0;
    char buffer[10];

    // We initialize a seen array that we will store all the cards we go through
    char seenCard[52][2] = {0};
    int seenCount = 0;


    while (i < 52 && fgets(buffer, sizeof(buffer), file) != NULL) {
        if (buffer[0] == '\n' || buffer[0] == '\0') {
            continue;
        }


        // Before we store each card in a "seen array" we check if it has already appeared and validate its content
        for (int j = 0; j < seenCount; j++) {
            if (seenCard[j][0] == buffer[0] && seenCard[j][1] == buffer[1]) {
                printf("LAST Command LD");
                printf("Message: Duplicate card found in deck file\n");
                fclose(file);
                return NULL;
            }
        }
        deck[i].rank = buffer[0];
        deck[i].suit = buffer[1];
        deck[i].visible = 0;

        //We store the cards we haven't been seen before
        seenCard[seenCount][0] = buffer[0];
        seenCard[seenCount][1] = buffer[1];

        seenCount++;

        i++;
    }

    fclose(file);
    // checks if there are exactly 52 cards
    if (i != 52) {
        printf("LAST Command LD\n");
        printf("Message: Deck file must contain exactly 52 cards\n");
        return NULL;
    }
    return arrayToList(deck, i);
}


int saveDeck(const char *filename, Card *head)
{
    printf("\nLast command: SD\n");
    if (head == NULL) {
        printf("Message: no deck loaded\n");
        return 0;
    }

    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        perror("Error saving deck");
        return 0;
    }

    Card *current = head;

    while (current != NULL) {
        fprintf(file, "%c%c\n", current->rank, current->suit); // putting in the deck
        current = current->next;
    }

    fclose(file);
    return 1;
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

    //Første kort er ikke random derfor
    int move = rand() % (sizeNewDeck - 1) + 1;   // Vælg et tilfældigt tal

    Card *first = shuffledDeck; //Pejer på første kort
    Card *target = shuffledDeck; //Pejer på den kort, det første kort skal placeres efter.

    for (int i = 1; i < move; i++) { //ittere hen til random plads
        target = target->next;
    }

    //pladsere kort
    shuffledDeck = first->next;
    first->next = target->next;
    target->next = first;

    //returnere head til kort
    return shuffledDeck;
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