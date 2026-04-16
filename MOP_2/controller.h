#ifndef CONTROLLER_H
#define CONTROLLER_H

void run_game();

typedef struct Card Card;
int readDeck(const char *filename, Card *deck);

#endif