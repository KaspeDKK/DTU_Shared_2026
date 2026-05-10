//
// Created by ttorr on 04-05-2026.
//
// TCP/IP Socket Server for Yukon Solitaire
// Listens on localhost:5000 and communicates with Python GUI

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../Model/model.h"
#include "../Model/deck.h"
#include "../Model/Types.h"
#include "../Controller/controller.h"
#include "../View/view.h"

// Platform-specific socket headers
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    #define CLOSE_SOCKET(s) closesocket(s)
    #define GET_SOCKET_ERROR() WSAGetLastError()
    #define SOCKET_INIT_ERROR -1
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    typedef int SOCKET;
    #define INVALID_SOCKET (-1)
    #define SOCKET_ERROR (-1)
    #define CLOSE_SOCKET(s) close(s)
    #define GET_SOCKET_ERROR() errno
    #define SOCKET_INIT_ERROR 0
#endif

#define PORT 5000
#define BUFFER_SIZE 4096

// Global game state
Column cols[7] = {0};
Foundation foundations[4] = {0};
Card *deckHead = NULL;
Card deck_array[52] = {0};
int gameStarted = 0;

// Helper: check if move was successful and build response
void handle_move_response(int moveResult, Foundation foundations[], char *response, size_t maxLen, const char *successMsg) {
    if (moveResult) {
        if (gameWon(foundations)) {
            snprintf(response, maxLen, "OK|GAME_WON");
        } else {
            snprintf(response, maxLen, "OK|%s", successMsg);
        }
    } else {
        snprintf(response, maxLen, "ERROR|Illegal move");
    }
}

// render board state as string
void render_board(char *output, size_t maxLen) {
    if (!gameStarted) {
        snprintf(output, maxLen, "Game not started");
        return;
    }

    char *pos = output;
    size_t remaining = maxLen;

    // Column headers
    int len = snprintf(pos, remaining, "C1    C2    C3    C4    C5    C6    C7     FOUNDATIONS\n");
    pos += len;
    remaining -= len;

    // Find max height
    int maxHeight = 0;
    for (int i = 0; i < 7; i++) {
        int h = 0;
        Card *c = cols[i].ref;
        while (c != NULL) {
            h++;
            c = c->next;
        }
        if (h > maxHeight) maxHeight = h;
    }

    int f = 0;

    // Print each row (columns on left, foundations on right)
    for (int row = 0; row < maxHeight && remaining > 0; row++) {
        Card *current[7];
        for (int i = 0; i < 7; i++) {
            current[i] = cols[i].ref;
            // Skip to the row we want
            for (int r = 0; r < row && current[i] != NULL; r++) {
                current[i] = current[i]->next;
            }
        }

        // Print columns
        for (int col = 0; col < 7 && remaining > 0; col++) {
            if (current[col] != NULL) {
                // Use * for hidden cards, space for visible
                char vis = current[col]->visible ? ' ' : '*';
                len = snprintf(pos, remaining, "%c[%c%c]  ", vis, current[col]->rank, current[col]->suit);
                current[col] = current[col]->next;
            } else {
                len = snprintf(pos, remaining, " [  ]  ");
            }
            pos += len;
            remaining -= len;
        }

        // Add foundation info on right side (one per row)
        if (f < 4 && remaining > 0) {
            len = snprintf(pos, remaining, "  F%d: ", f + 1);
            pos += len;
            remaining -= len;

            if (foundations[f].ref != NULL) {
                Card *last = foundations[f].ref;
                while (last->next != NULL) last = last->next;
                len = snprintf(pos, remaining, "[%c%c]", last->rank, last->suit);
            } else {
                len = snprintf(pos, remaining, "[  ]");
            }
            pos += len;
            remaining -= len;
            f++;
        }

        len = snprintf(pos, remaining, "\n");
        pos += len;
        remaining -= len;
    }

    // Print any foundations not covered by the main loop (when maxHeight < 4)
    while (f < 4 && remaining > 0) {
        for (int col = 0; col < 7 && remaining > 0; col++) {
            len = snprintf(pos, remaining, " [  ]  ");
            pos += len;
            remaining -= len;
        }

        len = snprintf(pos, remaining, "  F%d: ", f + 1);
        pos += len;
        remaining -= len;

        if (foundations[f].ref != NULL) {
            Card *last = foundations[f].ref;
            while (last->next != NULL) last = last->next;
            len = snprintf(pos, remaining, "[%c%c]", last->rank, last->suit);
        } else {
            len = snprintf(pos, remaining, "[  ]");
        }
        pos += len;
        remaining -= len;
        f++;

        len = snprintf(pos, remaining, "\n");
        pos += len;
        remaining -= len;
    }
}

// Process commands from the client
void process_command(const char *cmd, char *response, size_t maxLen) {
    // LD {filename} - Load Deck
    if (strncmp(cmd, "LD", 2) == 0) {
        char filename[256];
        memset(filename, 0, sizeof(filename));
        sscanf(cmd, "LD %255s", filename);

        if (strlen(filename) == 0) {
            strcpy(filename, "deckOne.txt");
        } else if (strstr(filename, ".txt") == NULL) {
            strcat(filename, ".txt");
        }

        gameStarted = 0;
        for (int i = 0; i < 7; i++) cols[i].ref = NULL;
        for (int i = 0; i < 4; i++) foundations[i].ref = NULL;

        memset(deck_array, 0, sizeof(deck_array));
        deckHead = readDeck(filename, deck_array);

        if (deckHead == NULL) {
            snprintf(response, maxLen, "ERROR|Failed to load deck: %s", filename);
        } else {
            snprintf(response, maxLen, "OK|Deck loaded: %s", filename);
        }
        return;
    }

    // SW - Show deck (display current deck state)
    if (strncmp(cmd, "SW", 2) == 0) {
        if (deckHead == NULL) {
            snprintf(response, maxLen, "ERROR|No deck loaded");
            return;
        }

        char *pos = response;
        size_t remaining = maxLen;
        int cardCount = 0;

        Card *current = deckHead;
        while (current != NULL && cardCount < 52 && remaining > 10) {
            int len = snprintf(pos, remaining, "[%c%c] ", current->rank, current->suit);
            if (len > 0) {
                pos += len;
                remaining -= len;
            }
            current = current->next;
            cardCount++;

            if (cardCount % 13 == 0 && remaining > 2) {
                *pos = '\n';
                pos++;
                remaining--;
            }
        }

        if (remaining > 20) {
            snprintf(pos, remaining, "\nTotal: %d cards", cardCount);
        }
        return;
    }

    // SR - Random Shuffle
    if (strncmp(cmd, "SR", 2) == 0) {
        if (deckHead == NULL) {
            snprintf(response, maxLen, "ERROR|No deck loaded");
            return;
        }

        deckHead = randomShuffle(deckHead);
        snprintf(response, maxLen, "OK|Deck shuffled randomly");
        return;
    }

    // SI {split} - Split deck
    if (strncmp(cmd, "SI", 2) == 0) {
        if (deckHead == NULL) {
            snprintf(response, maxLen, "ERROR|No deck loaded");
            return;
        }

        int splitPos = 0;
        sscanf(cmd, "SI %d", &splitPos);

        deckHead = splitDeck(deckHead, splitPos);
        if (deckHead != NULL) {
            snprintf(response, maxLen, "OK|Deck split at position %d", splitPos);
        } else {
            snprintf(response, maxLen, "ERROR|Failed to split deck");
        }
        return;
    }

    // SD {filename} - Save deck to file
    if (strncmp(cmd, "SD", 2) == 0) {
        if (deckHead == NULL) {
            snprintf(response, maxLen, "ERROR|No deck loaded");
            return;
        }

        char filename[256];
        char param[256] = {0};
        sscanf(cmd, "SD %255s", param);

        if (strlen(param) == 0) {
            snprintf(filename, sizeof(filename), "../cards.txt");
        } else {
            snprintf(filename, sizeof(filename), "../%s.txt", param);
        }

        if (saveDeck(filename, deckHead)) {
            snprintf(response, maxLen, "OK|Deck saved to %s", filename);
        } else {
            snprintf(response, maxLen, "ERROR|Failed to save deck to %s", filename);
        }
        return;
    }

    // P - Play (start game from loaded/shuffled deck)
    if (strncmp(cmd, "P", 1) == 0) {
        if (deckHead == NULL) {
            snprintf(response, maxLen, "ERROR|No deck loaded");
            return;
        }

        gameStarted = 0;

        // Reset columns and foundations
        for (int i = 0; i < 7; i++) {
            cols[i].ref = NULL;
        }
        for (int i = 0; i < 4; i++) {
            foundations[i].ref = NULL;
        }

        // Create game from current deck
        create_game(deckHead, cols);
        gameStarted = 1;
        snprintf(response, maxLen, "OK|Game started");
        return;
    }

    if (strncmp(cmd, "STATE", 5) == 0) {
        if (!gameStarted) {
            snprintf(response, maxLen, "STATE|Game not started");
            return;
        }
        char board[2048] = {0};
        render_board(board, sizeof(board));
        snprintf(response, maxLen, "STATE|%s", board);
        return;
    }

    if (strncmp(cmd, "MOVE", 4) == 0) {
        if (!gameStarted) {
            snprintf(response, maxLen, "ERROR|Game not started");
            return;
        }
        // Parse move string: "MOVE C1->C4" or "MOVE C3:7H->F1"
        const char *moveStr = cmd + 5;
        char from[32], to[32];
        sscanf(moveStr, "%[^-]->%s", from, to);

        if (to[0] == 'C') {
            // Destination is column - can come from column or foundation
            int toCol = to[1] - '0' - 1;

            if (toCol < 0 || toCol > 6) {
                snprintf(response, maxLen, "ERROR|Invalid column");
                return;
            }

            // Check if source is column or foundation
            if (from[0] == 'C') {
                // Column to column
                int fromCol = from[1] - '0' - 1;

                if (fromCol < 0 || fromCol > 6) {
                    snprintf(response, maxLen, "ERROR|Invalid column");
                    return;
                }

                // Check if specific card is requested (e.g., "col:card")
                Card *cardToMove = NULL;
                if (strchr(from, ':') != NULL) {
                    // Specific card format: "col:card"
                    char cardStr[3];
                    sscanf(from, "%*[^:]:%2s", cardStr);
                    cardToMove = cols[fromCol].ref;

                    // Search for card in the specified column
                    while (cardToMove != NULL && (cardToMove->rank != cardStr[0] || cardToMove->suit != cardStr[1])) {
                        cardToMove = cardToMove->next;
                    }

                    // Validate: card must be found AND must be visible
                    if (cardToMove == NULL) {
                        snprintf(response, maxLen, "ERROR|Card not found in source column");
                        return;
                    }
                    if (!cardToMove->visible) {
                        snprintf(response, maxLen, "ERROR|Cannot move a hidden card");
                        return;
                    }
                } else {
                    // Default to bottom card
                    cardToMove = getLastCard(cols[fromCol]);
                }

                if (cardToMove != NULL) {
                    int moveResult = moveCard(cardToMove, &cols[fromCol], &cols[toCol]);
                    handle_move_response(moveResult, foundations, response, maxLen, "Move successful");
                } else {
                    snprintf(response, maxLen, "ERROR|Source card not found");
                }
            } else if (from[0] == 'F') {
                // Foundation to column
                int fromFound = from[1] - '0' - 1;

                if (fromFound < 0 || fromFound > 3) {
                    snprintf(response, maxLen, "ERROR|Invalid foundation");
                    return;
                }

                Card *cardToMove = getLastCardFoundation(foundations[fromFound]);
                if (cardToMove != NULL) {
                    int moveResult = moveCardFromFoundation(cardToMove, &cols[toCol], &foundations[fromFound]);
                    handle_move_response(moveResult, foundations, response, maxLen, "Move from foundation successful");
                } else {
                    snprintf(response, maxLen, "ERROR|Foundation is empty");
                }
            } else {
                snprintf(response, maxLen, "ERROR|Invalid source");
                return;
            }
        } else if (to[0] == 'F') {
            // Column to foundation
            int toFound = to[1] - '0' - 1;

            if (toFound < 0 || toFound > 3) {
                snprintf(response, maxLen, "ERROR|Invalid foundation");
                return;
            }

            // Check if specific card is requested (e.g., "C7:QD")
            Card *cardToMove = NULL;
            if (strchr(from, ':') != NULL) {
                // Specific card format: "C7:QD"
                char colStr[3], cardStr[3];
                sscanf(from, "%[^:]:%2s", colStr, cardStr);
                int fromCol = colStr[1] - '0' - 1;
                if (fromCol < 0 || fromCol > 6) {
                    snprintf(response, maxLen, "ERROR|Invalid column");
                    return;
                }
                cardToMove = cols[fromCol].ref;

                // Search for card in the specified column
                while (cardToMove != NULL && (cardToMove->rank != cardStr[0] || cardToMove->suit != cardStr[1])) {
                    cardToMove = cardToMove->next;
                }

                // Validate: card must be found AND must be visible
                if (cardToMove == NULL) {
                    snprintf(response, maxLen, "ERROR|Card not found in source column");
                    return;
                }
                if (!cardToMove->visible) {
                    snprintf(response, maxLen, "ERROR|Cannot move a hidden card");
                    return;
                }

                int moveResult = moveCardFoundation(cardToMove, &cols[fromCol], &foundations[toFound]);
                if (moveResult) {
                    Card *newLast = getLastCard(cols[fromCol]);
                    if (newLast != NULL) {
                        newLast->visible = 1;
                    }
                }
                handle_move_response(moveResult, foundations, response, maxLen, "Move to foundation successful");
            } else {
                // Default to bottom card
                int fromCol = from[1] - '0' - 1;
                if (fromCol < 0 || fromCol > 6) {
                    snprintf(response, maxLen, "ERROR|Invalid column");
                    return;
                }
                cardToMove = getLastCard(cols[fromCol]);
                if (cardToMove != NULL) {
                    int moveResult = moveCardFoundation(cardToMove, &cols[fromCol], &foundations[toFound]);
                    if (moveResult) {
                        Card *newLast = getLastCard(cols[fromCol]);
                        if (newLast != NULL) {
                            newLast->visible = 1;
                        }
                    }
                    handle_move_response(moveResult, foundations, response, maxLen, "Move to foundation successful");
                } else {
                    snprintf(response, maxLen, "ERROR|Source column empty");
                }
            }
        } else {
            snprintf(response, maxLen, "ERROR|Invalid destination");
        }
        return;
    }

    if (strncmp(cmd, "QUIT", 4) == 0) {
        snprintf(response, maxLen, "OK|Goodbye");
        return;
    }

    snprintf(response, maxLen, "ERROR|Unknown command");
}

int main(void) {
    int iResult;
#ifdef _WIN32
    WSADATA wsaData;
     iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }
#endif

    SOCKET listenSocket = INVALID_SOCKET;
    SOCKET clientSocket = INVALID_SOCKET;

    // Create listening socket
    listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        printf("socket failed: %d\n", GET_SOCKET_ERROR());
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    // Allow reusing the address to avoid "Address already in use" error
    int reuseAddr = 1;
    if (setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuseAddr, sizeof(reuseAddr)) < 0) {
        printf("setsockopt failed: %d\n", GET_SOCKET_ERROR());
        CLOSE_SOCKET(listenSocket);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    // Bind to port
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(PORT);

    iResult = bind(listenSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (iResult == SOCKET_ERROR) {
        printf("bind failed: %d\n", GET_SOCKET_ERROR());
        CLOSE_SOCKET(listenSocket);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    // Start listening
    iResult = listen(listenSocket, 1);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed: %d\n", GET_SOCKET_ERROR());
        CLOSE_SOCKET(listenSocket);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    printf("Server listening on localhost:%d\n", PORT);

    // Accept and handle connections
    while (1) {
        struct sockaddr_in clientAddr;
#ifdef _WIN32
        int clientAddrLen = sizeof(clientAddr);
#else
        socklen_t clientAddrLen = sizeof(clientAddr);
#endif

        clientSocket = accept(listenSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSocket == INVALID_SOCKET) {
            printf("accept failed: %d\n", GET_SOCKET_ERROR());
            continue;
        }

        printf("Client connected\n");

        // Handle client commands
        char recvBuf[BUFFER_SIZE];
        char sendBuf[BUFFER_SIZE];

        while (1) {
            memset(recvBuf, 0, BUFFER_SIZE);
            memset(sendBuf, 0, BUFFER_SIZE);

            int iRecv = recv(clientSocket, recvBuf, BUFFER_SIZE - 1, 0);
            if (iRecv <= 0) {
                break; // Connection closed
            }

            // Null-terminate and trim newline
            recvBuf[iRecv] = '\0';
            if (iRecv > 0 && recvBuf[iRecv - 1] == '\n') {
                recvBuf[iRecv - 1] = '\0';
            }

            printf("Received: %s\n", recvBuf);

            // Process command
            process_command(recvBuf, sendBuf, BUFFER_SIZE - 2);

            // Send response with newline
            int responseLen = strlen(sendBuf);
            sendBuf[responseLen] = '\n';
            sendBuf[responseLen + 1] = '\0';

            int iSend = send(clientSocket, sendBuf, responseLen + 1, 0);
            if (iSend == SOCKET_ERROR) {
                printf("send failed: %d\n", GET_SOCKET_ERROR());
                break;
            }

            printf("Sent: %s\n", sendBuf);

            // Check for quit
            if (strncmp(recvBuf, "QUIT", 4) == 0) {
                break;
            }
        }

        CLOSE_SOCKET(clientSocket);
        printf("Client disconnected\n");
    }

    CLOSE_SOCKET(listenSocket);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}

