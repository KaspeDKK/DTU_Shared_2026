#include <stdio.h>
#include "view.h"

static void print_card(const Card* card)
{
    if (card == NULL) {
        printf("[E]");
        return;
    }
    if (card->visible == 1) {
        printf("[%c%c]", card->rank,card->suit);
    } else if (card->visible == 0) {
        printf("[]");
    }
}

static void print_card_face_up(const Card *card)
{
    if (card == NULL) return;
    printf("[%c%c]", card->rank, card->suit);
}

void print_deck(const Card *deck, const int size) //right now this takes an array - needs to be changed to *head -> linked list.
{
    for (int i = 0; i < size; i++) {
        printf("[%c%c] ", deck[i].rank, deck[i].suit); // print format, c and c
    }
    printf("\n");
}



void view_not_started() {
    int f = 1;
    int row = 0;

    printf("C1   C2   C3   C4   C5   C6   C7\n\n");

    for (int i = 0; i < 52; i++) {
        printf("[]   ");

        if ((i + 1) % 7 == 0) {
            if (row % 2 == 0 && f <= 4) {
                printf("     []   F%d", f);
                f++;
            }
            printf("\n");
            row++;
        }
    }
    printf("\nLAST Command: None\n");
    printf("Message: Enter Command\n");
}

void showGame(Column cols[]) //initial view of the deck, before the columns have been made.
{

}


void showCol(Card *head) {

    Card *current = head;
    int count = 0;

    while (current != NULL) {
        printf("\n");
        print_card_face_up(current);
        current = current->next;
        count++;
    }
}

void showDeck(Card *head) //initial view of the deck, before the columns have been made.
{
    if (head == NULL) {
        printf("LAST Command: SW\n");
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
                printf("     []   F%d", f);
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

        printf("\nLAST Command: SW\n");
        printf("Message: OK\n");
}
