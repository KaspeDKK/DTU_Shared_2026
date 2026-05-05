#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../Model/model.h"

void game_startup();
void processMove(char *input, Column cols[], Foundation foundations[]);
void run_game(Card *deckHead);

#endif