#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#include "../Controller/controller.h"
#include "../Model/Types.h"
#include "../Model/model.h"
#include "../View/view.h"

void run_game(Card *deckHead) {
    // new stucture
    Column cols[7] = {};
    Foundation foundations[4] = {};

    create_game(deckHead, cols); // model.c

    // print game
    debugShowGame(cols);

    int gameIsRunning = 0;

    while (gameIsRunning) {
        char input[100] = "";
        char from[20], to[20];

        // scan for input
        printf("Please enter your command: ");

        gets(input); // read user input

        // Remove newline from fgets
        input[strcspn(input, "\n")] = 0;

        // Parse the move command: split on "->"
        char *arrow = strstr(input, "->");
        if (arrow == NULL) {
            printf("Invalid command format. Use <from>-><to>\n");
            continue;
        }

        // Copy from and to parts
        size_t fromLen = arrow - input;
        strncpy(from, input, fromLen);
        from[fromLen] = '\0';
        strcpy(to, arrow + 2);  // Skip "->"

        printf("From: %s, To: %s\n", from, to);
        
        
        

            if (to[0] == 'C') {
            // Move to column
            int toCol = to[1] - '0' - 1;  // 'C4' -> index 3

            if (strchr(from, ':') != NULL) { //searches for char
                // Specific card: "C6:4H"
                char colStr[3], cardStr[3];
                sscanf(from, "%[^:]:%s", colStr, cardStr);
                int fromCol = colStr[1] - '0' - 1;
                Card moveToCard = parseCard(cardStr);
                // For now, assume single card move (extend for stacks later)
                moveCard(&moveToCard, &cols[fromCol], &cols[toCol]);
            } else if (from[0] == 'C') {
                // Bottom card of column: "C6"
                int fromCol = from[1] - '0' - 1;
                Card *bottomCard = getLastCard(cols[fromCol]);
                if (bottomCard != NULL) {
                    moveCard(bottomCard, &cols[fromCol], &cols[toCol]);
                }
            } else if (from[0] == 'F') {
                // Top card of foundation: "F3"
                int fromFound = from[1] - '0' - 1;
                Card *topCard = getLastCardFoundation(foundations[fromFound]);
                if (topCard != NULL) {
                    // Need a moveFoundationToColumn function
                    // For now, placeholder
                    printf("Foundation to column move not implemented yet\n");
                }
            }
        } else if (to[0] == 'F') {
            // Move to foundation
            int toFound = to[1] - '0' - 1;

            if (from[0] == 'C') {
                if (strchr(from, ':') != NULL) {
                    // Specific card from column to foundation
                    char colStr[3], cardStr[3];
                    sscanf(from, "%[^:]:%s", colStr, cardStr);
                    int fromCol = colStr[1] - '0' - 1;
                    Card moveToCard = parseCard(cardStr);
                    moveCardFoundation(&moveToCard, &cols[fromCol], &foundations[toFound]);
                } else {
                    // Bottom card of column to foundation
                    int fromCol = from[1] - '0' - 1;
                    Card *bottomCard = getLastCard(cols[fromCol]);
                    if (bottomCard != NULL) {
                        moveCardFoundation(bottomCard, &cols[fromCol], &foundations[toFound]);
                    }
                }
            }
        } else {
            printf("Invalid destination\n");
        }
    }
}

void game_startup()
{
    Card deck[52] = {}; // deck size 52
    Card *deckHead = readDeck("../deckOne.txt", deck); // standard deck
    showDeck(deckHead);

    // startup loop
    while (1) {

        char input[100] = "";
        char cmd[10], param[20];

        // scan for input
        printf("Please enter your command: ");

        gets(input); // read user input

        sscanf(input, "%s %s ", cmd, param);

        printf("Command: %s\n, Param: %s\n", cmd, param);

        if (strcmp(cmd, "LD") == 0) {
            // load the file using param

            // format the parameter into ../param.txt
            char filename[100];
            snprintf(filename, sizeof(filename), "../%s.txt", param);
            deckHead = readDeck(filename, deck);
            continue;
        }

        if (strcmp(cmd, "SW") == 0) {
            showDeck(deckHead);
            continue;
        }

        if (strcmp(cmd, "SI") == 0) {
            // validate that param contains only digits
            for (int i = 0; param[i] != '\0'; i++) {
                if (!isdigit((unsigned char)param[i])) {
                    printf("Error: split parameter must be a valid integer.\n");
                    break;
                }
            }
            const int param_int = atoi(param);  // convert to int

            splitDeck(deckHead, param_int);
            showDeck(deckHead);
            continue;
        }

        if (strcmp(cmd, "SR") == 0) {
            // random shuffle
            deckHead = randomShuffle(deckHead);
            showDeck(deckHead);
            continue;
        }

        if (strcmp(cmd, "SD") == 0) {
            // save current deck to file. filename is param
            continue;
        }

        if (strcmp(cmd, "P") == 0) {
            // enter play phase
            run_game(deckHead);
            continue;
        }

        if (strcmp(cmd, "Q") == 0) {
            return;
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

Card parseCard(const char *cardStr) {
    Card card;
    card.rank = cardStr[0];
    card.suit = cardStr[1];
    card.visible = 1;  // Cards being moved should be visible
    card.next = NULL;
    return card;
}

void moveCard(Card *moveCard, Column *columnFrom, Column *columnTo) {
    Card* headCard = columnFrom->ref;
    Card* endOfColumn = getLastCard(*columnTo);

    while (headCard->next != NULL && (headCard->next->rank != moveCard->rank || headCard->next->suit != moveCard->suit)) {
        headCard = headCard->next;
    }
    if (headCard->next == NULL) {
        printf("Picked card is not in column");
        return;
    }

    if (isMoveLegal(moveCard, endOfColumn)== 1) { //condition check
        Card* cardToMove = headCard->next;
        headCard->next = NULL;
        endOfColumn->next = cardToMove;
    } else {
        printf("Illegal move");
    }
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

void moveCardFoundation(Card *moveCard, Column *columnFrom, Foundation *foundation) {
    Card* headCard = columnFrom->ref;
    Card* endOfFoundation = getLastCardFoundation(*foundation);

    while (headCard->next->rank != moveCard->rank || headCard->next->suit != moveCard->suit) {
        headCard = headCard->next;
    }
    if (isMoveLegalFoundation(moveCard, headCard)== 1) { //condition check
        Card* cardToMove = headCard->next;
        headCard->next = NULL;
        endOfFoundation->next = cardToMove;
    } else {
        printf("Illegal move");
    }
}

void moveCardFromFoundation(Card *moveCard, Column columnFrom, Foundation foundation) {
    Card* headCard = columnFrom.ref;
    Card* endOfFoundation = getLastCardFoundation(foundation);

    while (headCard->next->rank != moveCard->rank || headCard->next->suit != moveCard->suit) {
        headCard = headCard->next;
    }
}

int determineRank(Card card) {
    char cardRank = card.rank;
    int cardRankInt;

    if (cardRank == 'A'){cardRankInt = 1;}
    else if (cardRank == 'K'){cardRankInt = 13;
    } else if (cardRank == 'Q'){cardRankInt = 12;
    } else if (cardRank == 'J'){cardRankInt = 11;
    } else if (cardRank == 'T'){cardRankInt = 10;
    } else {
        sscanf(&cardRank, "%d", &cardRankInt);
    }
    return cardRankInt;
}

int isMoveLegal(Card* moveCard, Card* cardTo) {
    int cardRank = determineRank(*moveCard);
    int cardRank2 = determineRank(*cardTo);

    if (moveCard->suit == cardTo->suit) {return 0;}
    if (cardRank != cardRank2+1) {return 0;}

    return 1;
}

int isMoveLegalFoundation(Card* moveCard, Card* cardTo) {
    int cardRank = determineRank(*moveCard);
    int cardRank2 = determineRank(*cardTo);

    if (moveCard->suit != cardTo->suit) {return 0;}
    if (cardRank != cardRank2+1) {return 0;}

    return 1;
}


// Tjek for spillet er vundet (condition)
int gameWon (Foundation foundation[NUM_FOUNDATIONS]) {
    for (int i = 0; i < NUM_FOUNDATIONS; i++) {
        if (foundation[i].ref == NULL || // Null checks! Ellers crasher den?
            determineRank(*getLastCardFoundation(foundation[i])) != 13) {
            return 0;
        }
    }
    return 1; // Alle 4 foundations er færdige

}
// Vi burde nok lave en convert rank to int metode, så vi slipper for 100 linjer ekstra kode :)
// DONE