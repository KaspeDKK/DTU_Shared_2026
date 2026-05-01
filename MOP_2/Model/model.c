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

// we need to make a function that takes a deck of 52 cards. Aka a linked list of 52 cards. The input will just be the deckhead and then we use that

// we will have to loop across all 7 cols inserting one card per col then switching to next one. When we reach the 7th col (%7==0) then we will go back to the first.

    // when inserting a card, we need to look at the former one. and set thats reference to the card we are inserting. If there are no cards in the list we ofc still need a ref. That will be the cols own ref.

// repeat until all cards from the original deck linked list are gone.

// later we implement rules that say other times we need to switch earlier to the next col.

void create_game(struct Card *deckHead, struct  Column cols[]) {
    Card *current = deckHead; // first card

    // somethings that stops when there are no more cards in the deckhead linked list

    for (int i = 0; i < 52; i++) {

        *current = *current->next; // next card in the pile

        int rowCount = 0;
        int columnCount = 0;
        int colStart = 0;
        // when row counter is 6 i needs to start at 1 instead of 0.

        // insert card at the bottom of the current column
        placeCard(cols[columnCount], current); // insert current card at given column list

        columnCount++;

        if (columnCount%7==0) {
            rowCount++;
        }

        if (rowCount==1) {
            colStart++;
        }

        if (rowCount==6) {
            colStart++;
        }
    }



}
