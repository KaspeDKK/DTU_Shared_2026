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

// Check if game is won
int gameWon (Foundation foundations[]) {
    for (int i = 0; i < NUM_FOUNDATIONS; i++) {
        if (foundations[i].ref == NULL || // Null checks required to avoid crash on empty foundation
            determineRank(*getLastCardFoundation(foundations[i])) != 13) {
            return 0;
            }
    }
    return 1; // All 4 foundations end on a King
}

void processMove(char *input, Column cols[], Foundation foundations[]) {
    char from[20], to[20];

    // Parse the move command: split on "->"
    char *arrow = strstr(input, "->");
    if (arrow == NULL) {
        printf("Invalid command format. Use <from>-><to>\n");
        return;
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

        if (strchr(from, ':') != NULL) { // Check if a specific card is given like "C6:4H"
            char colStr[3], cardStr[3];
            // Split "C6:4H" into column and card parts
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
            // Top card of from column
            int fromCol = from[1] - '0' - 1;
            Card *bottomCard = getLastCard(cols[fromCol]);
            if (bottomCard != NULL) {
                moveCard(bottomCard, &cols[fromCol], &cols[toCol]);
            }
            if (cols[fromCol].ref != NULL) getLastCard(cols[fromCol])->visible = 1;
        } else if (from[0] == 'F') {
            // Top card of foundation
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
    Column cols[7] = {};
    Foundation foundations[4] = {};

    create_game(deckHead, cols); // model.c

    while (1) {
        char input[100] = "";

        showGame(cols, foundations);

        printf("\nINPUT: ");
        gets(input);

        // Remove newline from fgets
        input[strcspn(input, "\n")] = 0;
        if (strcmp(input, "Q") == 0) {
            printf("Quitting game. Returning to startup phase\n");
            return;
        }

        processMove(input, cols, foundations);

        // Bruteforce visibility - may be deleted later
        showAllLastCards(cols);

        // Game winning condition
        if (gameWon(foundations)) {
            showGame(cols, foundations);
            printf("\nYou have won!\n");
            break;
        }
    }
}

void game_startup()
{
    Card deck[52] = {};
    Card *deckHead = NULL;
    showDeck(deckHead);


    // Startup loop
    while (1) {

        char input[100] = "";
        char cmd[10] = "";
        char param[20] = "";

        printf("Please enter your command: ");

        gets(input);

        // Split input into command and parameter
        sscanf(input, "%s %s ", cmd, param);

        printf("Command: %s\n, Param: %s\n", cmd, param);

        if (strcmp(cmd, "LD") == 0) {
            // Load the file using param

            // Format the parameter into ../param.txt
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
            // Validate that param contains only digits
            for (int i = 0; param[i] != '\0'; i++) {
                if (!isdigit((unsigned char)param[i])) {
                    printf("Error: split parameter must be a valid integer.\n");
                    break;
                }
            }
            const int param_int = atoi(param);  // Convert to int

            splitDeck(deckHead, param_int);
            showDeckWithVisibility(deckHead);
            printf("\nLast command: %s\n", cmd);
            printf("Message: OK\n");
            continue;
        }

        if (strcmp(cmd, "SR") == 0) {
            // Random shuffle
            deckHead = randomShuffle(deckHead);
            showDeckWithVisibility(deckHead);
            printf("\nLast command: %s\n", cmd);
            printf("Message: OK\n");
            continue;
        }

        if (strcmp(cmd, "SD") == 0) {
            // Save current deck to file. Filename is param
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
            // Enter play phase outside while loop
            break;
        }

        // This needs to be the last command
        if (strcmp(cmd, "QQ") == 0) {
            exit(0);
        } else {
            printf("No such command exists!\n");
        }

    }

    run_game(deckHead);
    game_startup();

}