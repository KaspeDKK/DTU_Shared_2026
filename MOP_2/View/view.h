#ifndef VIEW_H
#define VIEW_H

#include "../Model/Types.h"
#include "../Model/deck.h"

void showGame(Column cols[], Foundation foundations[]);
void showDeck(Card *head);
static void print_card_face_up(const Card *card);
Card *getRowCard(Card *colHead, int row);
int getColumnLength(Card *head);
int getMaxRows(Column cols[]);
#endif