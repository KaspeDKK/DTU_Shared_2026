#include <stdio.h>
#include <string.h>
#include "controller.h"

#include <ctype.h>
#include <stdlib.h>

#include "model.h"
#include "view.h"

void run_game()
{
    Card deck[52] = {};
    // TODO LOTS OF STUFF
    readDeck(".\\deckOne.txt", deck);

    print_deck(deck, 52);

<<<<<<< Updated upstream

    //test conversion
    char *deckArr = convertToArray(deck);

    print_deck(deckArr, 52);

=======
    view_not_started();


>>>>>>> Stashed changes
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
    return i;
}

Card* shuffleDeck (char arrDeck[]) {}

Card* convertTolinkedList (char *str) {

    Card *head = NULL;
    Card *current = NULL;

    for (int i = 0; str[i] != '\0'; i++) { // '\0' repræsentere slutningen af arrayet

        Card *newCard = malloc(sizeof(Card)); //reserver plads til kortet

        newCard->rank = str[i];
        i++;
        newCard->suit = str[i];
        newCard->next = NULL;

        if (head == NULL) //hvis det er første kort
        {
            head = newCard;
            current = newCard;
        }
        else //hvis det ikke er første kort
        {
            current->next = newCard;
            current = newCard;
        }
    }

    return head;
}

char *convertToArray(Card *deck) {
    char *arrDeck = malloc(105); // assuming 52

    int j = 0;
    Card *current = deck;

    while (current != NULL) {
        arrDeck[j++] = current->rank;
        arrDeck[j++] = current->suit;

        current = current->next;
    }

    arrDeck[j] = '\0';
    return arrDeck;
}