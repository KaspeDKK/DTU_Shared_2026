#ifndef VIEW_H
#define VIEW_H

#include "../Model/model.h"
#include "../Model/Card.h"


// TODO
// void view_print_game(const Game* game, const char* last_command, const char* message);


void print_deck(const Card *deck, int size);
void view_not_started();
void showDeck(Card *head);
static void print_card_face_up(const Card *card);
#endif