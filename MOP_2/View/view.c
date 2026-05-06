#include <stdio.h>
#include "view.h"
#include "../Controller/CardServices.h"

static void print_card(const Card* card)
{
    if (card == NULL) {
        printf("[E]");
        return;
    }
    if (card->visible == 1) {
        printf("[%c%c]", card->rank,card->suit);
    } else {
        printf("[  ]");
    }
}

static void print_card_face_up(const Card *card)
{
    if (card == NULL) return;
    printf("[%c%c]", card->rank, card->suit);
}

void showGame(Column cols[], Foundation foundations[])
{
    int maxRows = getMaxRows(cols);
    int f = 0; // foundation index, incremented as we place them

    printf("C1    C2    C3    C4    C5    C6    C7\n\n");

    for (int row = 0; row < maxRows; row++) {
        // print columns
        for (int col = 0; col < 7; col++) {
            Card *card = getRowCard(cols[col].ref, row);

            if (card != NULL) {
                print_card(card);  // respects visible property
            } else {
                printf("    ");
            }
            printf("  ");
        }

        // print foundations on odd rows, one per row
        if (row % 2 == 1 && f < 4) {
            Card *top = getLastCardFoundation(foundations[f]);
            if (top != NULL) {
                printf("     ");
                print_card_face_up(top);  // foundations always face up
                printf("   F%d", f + 1);
            } else {
                printf("     [  ]   F%d", f + 1);
            }
            f++;
        }

        printf("\n");
    }

    if (maxRows % 2 == 0) {
        printf("\n");
    }

    // Print foundations, så vi stadig kan se dem, når kolonnerne bliver mindre
    while (f < 4) {

        for (int col = 0; col < 7; col++) {
            printf("      ");
        }
        Card *top = getLastCardFoundation(foundations[f]);

        if (top != NULL) {
            printf("     ");
            print_card_face_up(top);
            printf("   F%d", f+1);
        } else {
            printf("     [  ]   F%d", f+1);
        }
        f++;
        printf("\n\n");
    }

    // edge case: if all columns are empty, still show foundations
    if (maxRows == 0) {
        for (int i = 0; i < 4; i++) {
            Card *top = getLastCardFoundation(foundations[i]);
            if (top != NULL) {
                print_card_face_up(top);
            } else {
                printf("[  ]");
            }
            printf("   F%d\n", i + 1);
        }
    }
}

void showDeck(Card *head) //initial view of the deck, before the columns have been made.
{
    if (head == NULL) {
        printf("C1    C2    C3    C4    C5    C6    C7\n\n");

        printf("                                             [  ]   F1\n\n");
        printf("                                             [  ]   F2\n\n");
        printf("                                             [  ]   F3\n\n");
        printf("                                             [  ]   F4\n\n");
        return;
    }

    printf("C1    C2    C3    C4    C5    C6    C7\n\n");

    Card *current = head;
    int count = 0;
    int row = 0;
    int f = 1;

    while (current != NULL) {
        print_card_face_up(current);
        count++;
        row++;

        if (count % 7 == 0) {
            if (row % 2 == 1 && f <= 4) {
                printf("     [  ]   F%d", f);
                f++;
            }
            printf("\n");
        } else {
            printf("  ");
            }

        current = current->next;
        }

        if (count % 7 != 0) {
            printf("\n");
        }

}

void showDeckWithVisibility(Card *head) //initial view of the deck, before the columns have been made.
{
    if (head == NULL) {
        return;
    }

    printf("C1    C2    C3    C4    C5    C6    C7\n\n");

    Card *current = head;
    int count = 0;
    int row = 0;
    int f = 1;

    while (current != NULL) {
        print_card(current);
        count++;
        row++;

        if (count % 7 == 0) {
            if (row % 2 == 1 && f <= 4) {
                printf("     [  ]   F%d", f);
                f++;
            }
            printf("\n");
        } else {
            printf("  ");
        }

        current = current->next;
    }

    if (count % 7 != 0) {
        printf("\n");
    }
}

Card *getRowCard(Card *colHead, int row) {
    Card *current = colHead;

    for (int i = 0; i < row; i++) {
        if (current == NULL) {
            return NULL;
        }
        current = current->next;
    }

    return current;
}

int getColumnLength(Card *head) // we just traverse the col (linked list) till bottom is found and note down the count
{
    int count = 0;

    while (head != NULL) {
        count++;
        head = head->next;
    }

    return count;
}

int getMaxRows(Column cols[]) // traverse each col in the game with the get col length func
{
    int max = 0;

    for (int i = 0; i < 7; i++) {
        int len = getColumnLength(cols[i].ref);

        if (len > max) {
            max = len;
        }
    }

    return max;
}