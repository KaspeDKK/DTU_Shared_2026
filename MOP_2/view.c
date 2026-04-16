#include <stdio.h>
#include "view.h"

static void print_card(const Card* card)
{
    /* TODO:
       Print:
       [] for hidden card
       rank+suit for visible card
       blank space for no card
    */
}

void print_deck(const Card *deck, const int size)
{
    for (int i = 0; i < size; i++) {
        printf("[%c%c] ", deck[i].rank, deck[i].suit);
    }
    printf("\n");
}

void view_not_started() {
    int f = 1;
    int k = 0;
    printf("C1   C2   C3   C4   C5   C6   C7 \n\n");
    for (int i = 0; i < 7; i++) {
        printf("[]   []   []   []   []   []   []");
        if (k % 2 == 0) {
            printf("     []   F%d",f);
            f++;
        }
        k++;
        printf("\n");
    }
    printf("LAST Command: None \n");
    printf("Message: Enter Command");
}