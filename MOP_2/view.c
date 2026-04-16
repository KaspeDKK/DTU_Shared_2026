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
        printf("[%c%c] ", deck[i].rank, deck[i].suit); // print format, c and c
    }
    printf("\n");
}

void view_print_game(const Game* game, const char* last_command, const char* message)
{
    // TODO
}