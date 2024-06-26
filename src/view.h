#ifndef MASKIN_PROJEKT2_VIEW_H
#define MASKIN_PROJEKT2_VIEW_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include "deck.h"
#include "Linkedlists.h"

void showDeck(Linked_list *deck_list, char command[], char statusMessage[]);

void emptyView(char lastCommand[], char message[]);

int calculateMaxRows(Linked_list *columns[]);

void generatePlayView(Linked_list *C_ptr[7], Linked_list *F_ptr[4], char lastCommand[], char message[]);

void clearView();

void generateColumns();

void printCC(char lastCommand[], char message[]);

#endif 
