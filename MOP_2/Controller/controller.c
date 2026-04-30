#include <stdio.h>
#include <string.h>
#include "../Controller/controller.h"

#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#include "../Model/model.h"
#include "../View/view.h"

void start_game(Card *deckHead) {
    // new stucture
    Column cols[7] = {};
    create_game(deckHead, cols); // model.c

    // game loop
}

void run_game()
{
    Card deck[52] = {}; // deck size 52
    // TODO LOTS OF STUFF
    Card *deckHead = readDeck("../deckOne.txt", deck);



    showDeck(deckHead);

    deckHead = randomShuffle(deckHead);
    deckHead = randomShuffle(deckHead);

    showDeck(deckHead);

    // startup loop
    while (1) {

        char input[] = "LD filename";
        char cmd[2], param[20];

        // scan for input
        printf("Please enter your command: ");

        gets(input); // read user input

        sscanf(input, "%s %s ", cmd, param);

        printf("Command: %s\n, Param: %s\n", cmd, param);

        if (strcmp(cmd, "LD") == 0) {
            // load the file using param
        }

        if (strcmp(cmd, "SW") == 0) {
            showDeck(deckHead);
        }

        if (strcmp(cmd, "SI") == 0) {
            // split shuffle using param as the split parameter
        }

        if (strcmp(cmd, "SR") == 0) {
            // random shuffle
        }

        if (strcmp(cmd, "SD") == 0) {
            // save current deck to file. filename is param
        }

        if (strcmp(cmd, "P") == 0) {
            // enter play phase
            start_game(deckHead);
        }

        // this needs to be the last command
        if (strcmp(cmd, "QQ") == 0) {
            exit(0);
        } else {
            printf("No such command exists!\n");
        }
    }

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

void moveCard(Card *moveCard, Column columnFrom, Column columnTo) {
    Card* headCard = columnFrom.ref;
    Card* endOfColumn = getLastCard(columnTo);

    while (headCard->next->rank != moveCard->rank && headCard->next->suit != moveCard->suit) {
        if (headCard->next == NULL) {
            printf("Picked card is not in column");
            return;
        }
        headCard = headCard->next;
    }

    headCard->next = NULL;

    endOfColumn->next = moveCard;
}

Card* getLastCard(Column column) {
    while (column.ref->next != NULL) {
        column.ref = column.ref->next;
    }
    return column.ref;
}

Card* getLastCardFoundation(Foundation foundation) {
    while (foundation.ref->next != NULL) {
        foundation.ref = foundation.ref->next;
    }
    return foundation.ref;
}

void moveCardFoundation(Card *moveCard, Column columnFrom, Foundation foundation) {
    Card* headCard = columnFrom.ref;
    Card* endOfFoundation = getLastCardFoundation(foundation);

    while (headCard->next->rank != moveCard->rank && headCard->next->suit != moveCard->suit) {
        headCard = headCard->next;
    }

    headCard->next = NULL;

    endOfFoundation->next = moveCard;
}

int isMoveLegal(Card* moveCard, Card* cardTo) {
    if (moveCard->suit != cardTo->suit) {
        return 0;
    }

    char cardRank1 = moveCard->suit;
    int cardRankInt1;
    sscanf(&cardRank1, "%d", &cardRankInt1);

    char cardRank2 = cardTo->suit;
    int cardRankInt2;
    sscanf(&cardRank2, "%d", &cardRankInt2);

    if (cardRank1 != cardRank2+1) {
        return 0;
    }
    return 1;
}