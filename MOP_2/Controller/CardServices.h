//
// Created by kaspe on 30-04-2026.
//

#ifndef MOP_2_CARDSERVICES_H
#define MOP_2_CARDSERVICES_H

#include "../Model/model.h"

#include "../Model/Types.h"

void placeCard(Column *column, Card *card);
Card* getLastCard(Column column);
Card* getLastCardFoundation(Foundation foundation);
int isMoveLegal(Card* moveCard, Card* cardTo);
int isMoveLegalFoundation(Card* moveCard, Card* cardTo);
Card parseCard(const char *cardStr);
void moveCardFoundation(Card *moveCard, Column *columnFrom, Foundation *foundation);
void moveCardFromFoundation(Card *moveCard, Column *columnTo, Foundation *fromFoundation);
void moveCard(Card *moveCard, Column *columnFrom, Column *columnTo);
int determineRank(Card card);

#endif //MOP_2_CARDSERVICES_H