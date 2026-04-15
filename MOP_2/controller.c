#include <stdio.h>
#include <string.h>
#include "controller.h"
#include "model.h"
#include "view.h"

void run_game()
{
    // TODO LOTS OF STUFF
}

int readDeck (const char *filename,Card *deck) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    int i = 0;
  
    char buffer[10];

    while (fgets(buffer, 10, file) != NULL) {
        deck[i].rank = buffer[0];
        deck[i].suit = buffer[1];
        deck[i].visible = 0;
        i++;
    }

    fclose(file);
    return 0;
}