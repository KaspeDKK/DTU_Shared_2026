//
// Created by kaspe on 30-04-2026.
//

#ifndef MOP_2_CARDSERVICES_H
#define MOP_2_CARDSERVICES_H

#include "../Model/model.h"

#include "../Model/Types.h"

// Services for manipulating cards, columns, and foundations. used to setup game. Argument: column or foundation to place card in, and card to place.
void placeCard(Column *column, Card *card);
//gets last card in column. Returns Card* or NULL if column is empty. Argument is column to check.
Card* getLastCard(Column column);
//gets last card in foundation. returns Card* or NULL if foundation is empty. Argument is foundation to check.
Card* getLastCardFoundation(Foundation foundation);
//checks whether or not the move respects rules of the game. Returns 1 if legal, 0 if not. Arguments are the card being moved and the card it is being moved to.
int isMoveLegal(Card* moveCard, Card* cardTo);
//checks whether or not the move respects rules of the game. Returns 1 if legal, 0 if not. Arguments are the card being moved and the card it is being moved to.
int isMoveLegalFoundation(Card* moveCard, Card* cardTo);
//Parse a char string into a card struct. returns a new Card. Argument is a Char
Card parseCard(const char *cardStr);
//Moves a card to a foundation. Returns 1 if move is successful 0 if not. Arguments are the card to move, the column it is moving from, and the foundation it is moving to.
int moveCardFoundation(Card *moveCard, Column *columnFrom, Foundation *foundation);
//Moves a card from a foundation to a column. Returns 1 if move is successful 0 if not. Arguments are the card to move, the column it is moving to, and the foundation it is moving from.
int moveCardFromFoundation(Card *moveCard, Column *columnTo, Foundation *fromFoundation);
//Moves a card from a column to a new column. Returns 1 if move is successful 0 if not. Arguments are the card to move, the column it is moving from, and the column it is moving to.
int moveCard(Card *moveCard, Column *columnFrom, Column *columnTo);
//Determines rank as a number. I.e J == 11. returns int, Argument is card.
int determineRank(Card card);
//A brute force command, that goes through all columns and makes the last card of each visible. Used for edge cases. Argument is columns.
void showAllLastCards(Column cols[]);

#endif //MOP_2_CARDSERVICES_H