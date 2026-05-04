#ifndef VIEW_H
#define VIEW_H

#include "../Model/Types.h"
#include "../Model/deck.h"


// TODO
// void view_print_game(const Game* game, const char* last_command, const char* message);


void print_deck(const Card *deck, int size);
void view_not_started();
void showCol(Card *head);
void debugShowGame(Column cols[], Foundation foundations[]);
void showGame(Column cols[]);
void showDeck(Card *head);
static void print_card_face_up(const Card *card);
Card *getRowCard(Card *colHead, int row);
int getColumnLength(Card *head);
int getMaxRows(Column cols[]);
#endif