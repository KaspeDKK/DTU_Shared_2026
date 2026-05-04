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

void run_game(Card *deckHead) {
    // new stucture
    Column cols[7] = {};
    Foundation foundations[4] = {};

    create_game(deckHead, cols); // model.c

    int gameIsRunning = 1;

    while (gameIsRunning) {
        char input[100] = "";
        char from[20], to[20];

        // print game
        debugShowGame(cols,foundations); // NEEDS TO BE CHANGED TO NORMAL FUNC LATER

        // scan for input
        printf("\nINPUT: ");

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