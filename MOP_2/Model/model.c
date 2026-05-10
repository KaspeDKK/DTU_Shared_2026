#include <stdlib.h>
#include <string.h>
#include "model.h"

/*
 * create_game
 *
 * The function takes a deck of 52 cards (linked list) as deckHead and distributes them into 7 columns.
 *
 * We loop across all 7 cols inserting one card per col then switching to the next one.
 * Cards are inserted using placeCard which handles placing the card correctly in the column
 * We repeat until all cards from the original deck linked list are gone.
*/
void create_game(Card *deckHead, Column cols[]) {
    Card *current = deckHead;

    int startColPerRow[11] = {
        0, 1, 1, 1, 1, 1, 2, 3, 4, 5, 6 // these represent the "col start" value for each row. These are always the same.
    };

    int colInvisibleDepth[7] = {
        // these represent the depth after which visible cards should be placed in the initial version of the game deck.
        // (All cards are invisible when created unless set otherwise)
        0, 1, 2, 3, 4, 5, 6
    };

    for (int row = 0; row < 11; row++) { // rowwise
        for (int col = startColPerRow[row]; col < 7; col++) { // uses the start col values to know when to stop using a column
            if (current == NULL) return;

            Card *next = current->next;
            current->next = NULL;

            // setting visibility before placing
            if (row < colInvisibleDepth[col]) {
                current->visible = 0;
            } else {
                current->visible = 1;
            }

            placeCard(&cols[col], current);

            current = next;
        }
    }
}
