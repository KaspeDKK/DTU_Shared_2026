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

/*
 * create_game
 *
 *
 * we need to make a function that takes a deck of 52 cards. Aka a linked list of 52 cards. The input will just be the deckhead and then we use that
 *
 * we will have to loop across all 7 cols inserting one card per col then switching to next one. When we reach the 7th col (%7==0) then we will go back to the first.
 *     when inserting a card, we need to look at the former one. and set thats reference to the card we are inserting. If there are no cards in the list we ofc still need a ref. That will be the cols own ref.
 * repeat until all cards from the original deck linked list are gone.
 * later we implement rules that say other times we need to switch earlier to the next col.
*/
void create_game(Card *deckHead, Column cols[]) {
    Card *current = deckHead;

    int startColPerRow[11] = {
        0, 1, 1, 1, 1, 1, 2, 3, 4, 5, 6 // these represent the "col start" value for each row. These are always the same.
    };

    for (int row = 0; row < 11; row++) {
        for (int col = startColPerRow[row]; col < 7; col++) {
            if (current == NULL) return;

            Card *next = current->next;
            current->next = NULL;

            placeCard(&cols[col], current);

            current = next;
        }
    }
}
