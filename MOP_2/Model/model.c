#include <stdlib.h>
#include <string.h>
#include "model.h"

// TODO LOTS OF STUFF

struct Card *getNth(Card *deckHead, int n) {
    Card *current = deckHead;
    int count = 0;

    // Traverse the list until we reach the nth node or the end
    while (current != NULL) {
        if (count == n) {
            // Found the nth card, return its value
            return current;
        }
        count++;
        current = current->next;
    }

    // If we reach here, n was out of bounds (larger than the deck size)
    // You should handle this based on your project requirements
    // (e.g., return an empty Card or exit)
    exit(1);
}

// create game
void create_game(Card *deckHead) {
    // setting up the game

    // loop over deckhead linked list and populate the col linked lists
    /*
    // a col is just a name and a ref to the first card
    Column cols[7] = {};
    for (int i = 0; i < 7; i++) {
        cols[i].ref = getNth(deckHead, i);
    }*/

    /* cards are placed in rows using the linked lists for cols. so c1, c2, c3...
    * each card places increments a counter
    * when the counter reaches these checkpoints, it will stop using cols like c1 and upwards for each checkpoint
    *
    * 1. on the 7th count it stops using c1
    * 2. on the
    */
}