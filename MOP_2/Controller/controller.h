#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../Model/model.h"

//Starts the startup phase for the game
void game_startup();
//Checks the winning condition of the game. Returns 1 if won, 0 if not. Argument is foundations.
int gameWon (Foundation foundations[]);
//takes input from console, parses input, and if valid, executes move. Arguments are the input string, columns, and foundations.
void processMove(char *input, Column cols[], Foundation foundations[]);
//initiates playing phase. Argument is head of the played deck. 
void run_game(Card *deckHead);

#endif