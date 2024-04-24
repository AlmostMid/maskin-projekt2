//
// Created by eren on 19/04/2024.
//

#ifndef MASKIN_PROJEKT2_DECK_H
#define MASKIN_PROJEKT2_DECK_H

#include <stdio.h>
#include <stdbool.h>
#include "Linkedlists.h"

#define SUIT_SIZE 13
typedef struct {
    char suit;
    char value;
    bool faceUp;
    bool exists;
} Card;

extern Card clubs [SUIT_SIZE];
extern Card diamonds [SUIT_SIZE];
extern Card hearts [SUIT_SIZE];
extern Card spades [SUIT_SIZE];

extern Card * deck[4];

void fillSuits();
Linked_list *loadDeck(FILE *);

void  saveDeck(Linked_list * list, FILE *fptr);

int checkCard(struct ListCard *deck_card);

int convertCardASCIItoDecimal(char vardValue);


#endif //MASKIN_PROJEKT2_DECK_H
