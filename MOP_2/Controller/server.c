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
#define BUFFER_SIZE 1024

// Global game state
Column cols[7] = {};
Foundation foundations[4] = {};
Card *deckHead = NULL;
int gameStarted = 0;

// Helper: render board state as string
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
        if (row < 4 && remaining > 0) {
            len = snprintf(pos, remaining, "  F%d: ", row + 1);
            pos += len;
            remaining -= len;

            if (foundations[row].ref != NULL) {
                Card *last = foundations[row].ref;
                while (last->next != NULL) last = last->next;
                len = snprintf(pos, remaining, "[%c%c]", last->rank, last->suit);
            } else {
                len = snprintf(pos, remaining, "[  ]");
            }
            pos += len;
            remaining -= len;
        }

        len = snprintf(pos, remaining, "\n");
        pos += len;
        remaining -= len;
    }
}

// Process commands from the client
void process_command(const char *cmd, char *response, size_t maxLen) {
    if (strncmp(cmd, "START", 5) == 0) {
        // Allow game restart
        gameStarted = 0;

        // Load deck
        Card deck_array[52] = {};
        deckHead = readDeck("../deckOne.txt", deck_array);
        if (deckHead == NULL) {
            snprintf(response, maxLen, "ERROR|Failed to load deck");
            return;
        }
        // Reset columns and foundations
        for (int i = 0; i < 7; i++) {
            cols[i].ref = NULL;
        }
        for (int i = 0; i < 4; i++) {
            foundations[i].ref = NULL;
        }
        // Create game
        create_game(deckHead, cols);
        gameStarted = 1;
        snprintf(response, maxLen, "OK|Game started");
        return;
    }

    if (strncmp(cmd, "STATE", 5) == 0) {
        if (!gameStarted) {
            snprintf(response, maxLen, "ERROR|Game not started");
            return;
        }
        char board[2048] = {};
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
            // Column to column
            int fromCol = from[1] - '0' - 1;
            int toCol = to[1] - '0' - 1;

            if (fromCol < 0 || fromCol > 6 || toCol < 0 || toCol > 6) {
                snprintf(response, maxLen, "ERROR|Invalid column");
                return;
            }

            // Check if specific card is requested (e.g., "C7:QD")
            Card *cardToMove = NULL;
            if (strchr(from, ':') != NULL) {
                // Specific card format: "C7:QD"
                char cardStr[3];
                sscanf(from, "%*[^:]:%2s", cardStr);
                cardToMove = cols[fromCol].ref;
                while (cardToMove != NULL && (cardToMove->rank != cardStr[0] || cardToMove->suit != cardStr[1])) {
                    cardToMove = cardToMove->next;
                }
            } else {
                // Default to bottom card
                cardToMove = getLastCard(cols[fromCol]);
            }

            if (cardToMove != NULL) {
                moveCard(cardToMove, &cols[fromCol], &cols[toCol]);
                snprintf(response, maxLen, "OK|Move successful");
            } else {
                snprintf(response, maxLen, "ERROR|Source card not found");
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
                while (cardToMove != NULL && (cardToMove->rank != cardStr[0] || cardToMove->suit != cardStr[1])) {
                    cardToMove = cardToMove->next;
                }
                if (cardToMove != NULL) {
                    moveCardFoundation(cardToMove, &cols[fromCol], &foundations[toFound]);
                    snprintf(response, maxLen, "OK|Move to foundation successful");
                } else {
                    snprintf(response, maxLen, "ERROR|Source card not found");
                }
            } else {
                // Default to bottom card
                int fromCol = from[1] - '0' - 1;
                if (fromCol < 0 || fromCol > 6) {
                    snprintf(response, maxLen, "ERROR|Invalid column");
                    return;
                }
                cardToMove = getLastCard(cols[fromCol]);
                if (cardToMove != NULL) {
                    moveCardFoundation(cardToMove, &cols[fromCol], &foundations[toFound]);
                    snprintf(response, maxLen, "OK|Move to foundation successful");
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
#ifdef _WIN32
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
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
            process_command(recvBuf, sendBuf, BUFFER_SIZE - 1);

            // Send response
            int iSend = send(clientSocket, sendBuf, (int)strlen(sendBuf), 0);
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

