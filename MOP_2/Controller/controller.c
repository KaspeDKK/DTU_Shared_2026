#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#include "../Controller/controller.h"
#include "../Model/Types.h"
#include "../Model/model.h"
#include "../Model/deck.h"
#include "../View/view.h"

// Tjek for spillet er vundet (condition)
int gameWon (Foundation foundations[]) {
    for (int i = 0; i < NUM_FOUNDATIONS; i++) {
        if (foundations[i].ref == NULL || // Null checks! Ellers crasher den?
            determineRank(*getLastCardFoundation(foundations[i])) != 13) {
            return 0;
            }
    }
    return 1; // Alle 4 foundations er færdige
}

void processMove(char *input, Column cols[], Foundation foundations[]) {
    char from[20], to[20];

    // Parse the move command: split on "->"
    char *arrow = strstr(input, "->");
    if (arrow == NULL) {
        printf("Invalid command format. Use <from>-><to>\n");
        return;
    }

    // copy from and to parts
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

            Card *cardToMove = cols[fromCol].ref;
            while (cardToMove != NULL && (cardToMove->rank != cardStr[0] || cardToMove->suit != cardStr[1])) {
                cardToMove = cardToMove->next;
            }
            if (cols[fromCol].ref != NULL) getLastCard(cols[fromCol])->visible = 1;
            if (cardToMove == NULL) {
                printf("Card not found in column\n");
            } else {
                moveCard(cardToMove, &cols[fromCol], &cols[toCol]);
            }
        } else if (from[0] == 'C') {
            // Bottom card of column: "C6"
            int fromCol = from[1] - '0' - 1;
            Card *bottomCard = getLastCard(cols[fromCol]);
            if (bottomCard != NULL) {
                moveCard(bottomCard, &cols[fromCol], &cols[toCol]);
            }
            if (cols[fromCol].ref != NULL) getLastCard(cols[fromCol])->visible = 1;
        } else if (from[0] == 'F') {
            // top card of foundation
            int fromFound = from[1] - '0' - 1;
            Card *topCard = getLastCardFoundation(foundations[fromFound]);
            if (topCard != NULL) {
                moveCardFromFoundation(topCard, &cols[toCol], &foundations[fromFound]);
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

                Card *cardToMove = cols[fromCol].ref;
                if (cols[fromCol].ref != NULL) getLastCard(cols[fromCol])->visible = 1;
                while (cardToMove != NULL && (cardToMove->rank != cardStr[0] || cardToMove->suit != cardStr[1])) {
                    cardToMove = cardToMove->next;
                }

                if (cardToMove == NULL) {
                    printf("Card not found in column\n");
                } else {
                    moveCardFoundation(cardToMove, &cols[fromCol], &foundations[toFound]);
                }
            } else {
                // Bottom card of column to foundation
                int fromCol = from[1] - '0' - 1;
                Card *bottomCard = getLastCard(cols[fromCol]);
                if (bottomCard != NULL) {
                    moveCardFoundation(bottomCard, &cols[fromCol], &foundations[toFound]);
                    if (cols[fromCol].ref != NULL) getLastCard(cols[fromCol])->visible = 1;
                }
            }
        }
    } else {
        printf("Invalid destination\n");
    }
}

void run_game(Card *deckHead) {
    // new structure
    Column cols[7] = {};
    Foundation foundations[4] = {};

    create_game(deckHead, cols); // model.c

    /* TESTING BELOW */
    int testing = 1; // ENABLE // DISABLE
    if (testing) {
        // replay moves JUST FOR TESTING
        const char *replay[] = {
            "C1->F1",
            "C3->C7",
            "C6->C7",
            "C3:7H->C5",
            "C3:AH->F2",
            "C2:8C->C6",
            "C6:TH->C7",
            "C7:7S->C3",
            "C7->C6",
            "C2->F1",
            "C3:KD->C1",
            "C1->C3",
            "C3:KS->C2",
            "C3->C1",
            "C7->C1",
            "C4:9D->C3",
            "C4->C3",
            "C2:9S->C4",
            "C7->C2",
            "C5:5S->C4",
            "C5->F3",
            "C2:6S->C4",
            "C2->F3",
            "C6->F3",
            "C3:4S->C4",
            "C7->C3",
            "C4:TC->C1",
            "C5:3H->C4",
            "C1:8S->C4",
            "C6->C1",
            "C3:TS->C6",
            "C7->C3",
            "C2:JS->C6",
            "C6:JD->C2",
            "C4:9H->C5",
            "C4:4C->C6",
            "C5:5H->C3",
            "C6:5D->C5",
            "C7->C4",
            "C2:QH->C4",
            "C6->C2",
            "C1:6H->C7",
            "C6->C1",
            "C4:KH->C6",
            "C7:7C->C4",
            "C3:2D->C5",
            "C4:8H->C3",
            "C4->F2",
            "C1:8C->C4",
            "C5:8D->C1",
            "C3:9C->C6",
            "C3->F1",
            "C1->C6",
            "C1->F3",
            "C5:4C->C1",
            "C6:4H->C5",
            "C6->F3",
            "C5:5D->C4",
            "C5->F3",
            "C1:6D->C5",
            "C1->F3",
            "C6:7C->C1",
            "C4:7D->C6",
            "C4->F4",
            "C5->F4",
            "C5->F2",

            "C6->F4",
            "C6->F2",
            "C5->F1",
            "C5->F2",

            "C1->F2",
            "C5->C1",
            "C5->F2",
            "C6:7D->C4",
            "C6->F2",
            "C5->F3",
            "C5->F2",
            "C6->C5",
            "C6->F2",
            "C5:TD->C6",
            "C5->F4",
            "C4->F4",
            "C1->F4",
            "C6:JS->C2",
            "C5->C6",
            "C5->F1",
            "C4->F1",


            "C1->F1",
            "C4->F4",

            "C4->F1",
            "C1->F4",

            //
            "C4->F4",
            "C1->F3",
            "C2->F1",
            "C4->F3",
            "C1->F1",
            "C2->F4",
            "C1->F2",
            "C2->F3",
            "C4->F4",
            "C6->F1",
            "C2->F1",
            "C4->F2",
            "C6->F4",
            "C1->F3",
            "C1->F4",
            "C2->F3",
            "C4->F1",
            //"C6->F2",
            NULL
        };

        for (int i = 0; replay[i] != NULL; i++) {
            char buf[100];
            strcpy(buf, replay[i]);
            processMove(buf, cols, foundations);
        }
    }
    /* ^^^^ TESTING ^^^^ */

    while (1) {
        char input[100] = "";

        // print game
        showGame(cols, foundations);

        // scan for input
        printf("\nINPUT: ");
        gets(input);

        // remove newline from fgets
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "Q") == 0) {
            printf("Quitting game. Returning to startup phase\n");
            return;
        }

        //initiate move
        processMove(input, cols, foundations);

        //bruteforce visibility - may be deleted later
        showAllLastCards(cols);

        //game winning condition
        if (gameWon(foundations)) {
            showGame(cols, foundations);
            printf("\nYou have won!\n");
            return;
        }
    }
}

void game_startup()
{
    Card deck[52] = {}; // deck size 52
    Card *deckHead = NULL;
    showDeck(deckHead);


    // startup loop
    while (1) {

        char input[100] = "";
        char cmd[10] = "";
        char param[20] = "";

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
            showDeckWithVisibility(deckHead);
            printf("\nLast command: %s\n", cmd);
            printf("Message: OK\n");
            continue;
        }

        if (strcmp(cmd, "SW") == 0) {
            showDeck(deckHead);
            printf("\nLast command: %s\n", cmd);
            printf("Message: OK\n");
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
            showDeckWithVisibility(deckHead);
            printf("\nLast command: %s\n", cmd);
            printf("Message: OK\n");
            continue;
        }

        if (strcmp(cmd, "SR") == 0) {
            // random shuffle
            deckHead = randomShuffle(deckHead);
            showDeckWithVisibility(deckHead);
            printf("\nLast command: %s\n", cmd);
            printf("Message: OK\n");
            continue;
        }

        if (strcmp(cmd, "SD") == 0) {
            // save current deck to file. filename is param
            char filename[100];

            if (strlen(param) == 0) {
                strcpy(filename, "../cards.txt");
            } else {
                snprintf(filename, sizeof(filename), "../%s.txt", param);
            }
            saveDeck(filename, deckHead);

            printf("\nLast command: %s\n", cmd);
            printf("Message: OK\n");
            continue;
        }

        if (strcmp(cmd, "P") == 0) {
            if (deckHead == NULL) {
                printf("\nNo deck loaded, please use LD <deck name> before initiating game\n");
                continue;
            }
            // enter play phase outside while loop
            break;
        }

        // this needs to be the last command
        if (strcmp(cmd, "QQ") == 0) {
            exit(0);
        } else {
            printf("No such command exists!\n");
        }

    }

    run_game(deckHead);
    game_startup();

}