#ifndef VIEW_H
#define VIEW_H

#include "../Model/Types.h"
#include "../Model/deck.h"

//Shows the game state in the console in playing phase. Arguments are columns and foundations.
void showGame(Column cols[], Foundation foundations[]);
//Show the initial deck state in the console in startup phase. Argument is head of the deck linked list.
void showDeck(Card *head);
//Shows a card whether it is visible. Argument is *Card
static void print_card_face_up(const Card *card);
//Shows cards for their respective row and column. Arguments are the head of the column and the row number.
Card *getRowCard(Card *colHead, int row);
// returns column length as INT, argument is head of column
int getColumnLength(Card *head);
// Returns max rows, argument is Columns
int getMaxRows(Column cols[]);
// Shows deck with respect to visibility. Argument is head of deck linked list.
void showDeckWithVisibility(Card *head);
#endif