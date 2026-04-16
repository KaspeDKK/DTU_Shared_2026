#ifndef VIEW_H
#define VIEW_H

#include "model.h"


// TODO
// void view_print_game(const Game* game, const char* last_command, const char* message);

typedef struct Card Card;
void print_deck(const Card *deck, int size);

#endif