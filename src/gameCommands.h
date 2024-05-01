#ifndef MASKIN_PROJEKT2_GAMECOMMANDS_H
#define MASKIN_PROJEKT2_GAMECOMMANDS_H

#include "Linkedlists.h"
#include "deck.h"
#include "view.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

Linked_list *LD(char arg[], int numOfInputs);

Linked_list *SR(Linked_list *unshuffledPile);

void SD(Linked_list *list, char arg[]);

Linked_list **P(Linked_list *);

Linked_list *SI(Linked_list *, int split);

bool isMoveValid(struct ListCard *from, struct ListCard *to, bool toFoundation);

bool gameMoves(char buf[], Linked_list **column_lists, Linked_list **foundation_lists);

int convertCardValue(char value);


#endif //MASKIN_PROJEKT2_GAMECOMMANDS_H
