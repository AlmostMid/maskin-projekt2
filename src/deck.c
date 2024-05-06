//
// Created by eren on 19/04/2024.
//

#include "deck.h"
#include "view.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Card clubs[SUIT_SIZE];
Card diamonds[SUIT_SIZE];
Card hearts[SUIT_SIZE];
Card spades[SUIT_SIZE];

Card *deck[4] = {clubs, diamonds, hearts, spades};

void fillSuit() {
    char suits[4] = {'C', 'D', 'H', 'S'};

    for (int i = 0; i < 4; i++) {
        deck[i][0].value = 'A';
        deck[i][0].suit = suits[i];
        deck[i][0].exists = false;

        for (int j = 1; j < 10; j++) {
            deck[i][j].value = '0' + (j + 1);
            deck[i][j].suit = suits[i];
            deck[i][j].exists = false;
        }

        deck[i][9].value = 'T';
        deck[i][9].suit = suits[i];
        deck[i][9].exists = false;
        deck[i][10].value = 'J';
        deck[i][10].suit = suits[i];
        deck[i][10].exists = false;
        deck[i][11].value = 'Q';
        deck[i][11].suit = suits[i];
        deck[i][11].exists = false;
        deck[i][12].value = 'K';
        deck[i][12].suit = suits[i];
        deck[i][12].exists = false;
    }
}

int checkCard(struct ListCard *deck_card) {
    int suit;
    switch (deck_card->suit) {
        case 'C' :
            suit = 0;
            break;
        case 'D' :
            suit = 1;
            break;
        case 'H' :
            suit = 2;
            break;
        case 'S' :
            suit = 3;
            break;
        default:
            return 2;
    }

    int value = convertASCII(deck_card->value);
    if (value == -1) return 2;

    if (!deck[suit][value].exists) {
        deck[suit][value].exists = true;
        deck_card->exists = true;
        return 0;
    } else return 1;
}

int convertASCII(char cardValue) {
    int value;
    if (cardValue == 'A' || cardValue == 'a') {
        value = 0;
    }
    else if (cardValue >= '1' && cardValue <= '9') {
        value = cardValue - '0' - 1;
    } else {
        switch (cardValue) {
            case 'T':
                value = 9;
                break;
            case 'J':
                value = 10;
                break;
            case 'Q':
                value = 11;
                break;
            case 'K':
                value = 12;
                break;
            default:
                return -1;
        }
    }

    return value;
}

Linked_list *loadDeck(FILE *fptr) {
    fillSuit();
    char line[4];
    Linked_list *cardDeck = createLinkedList();

    int lineNum = 1;
    while (fgets(line, sizeof(line), fptr) != NULL) {
        struct ListCard newCard;
        newCard.value = line[0];
        newCard.suit = line[1];

        char buffer[40];
        char *num;

        int check = checkCard(&newCard);

        if (check != 0 && sprintf(&num, "%d", lineNum) != -1) {
            if (check == 1)
                strcat(strcpy(buffer, "ERROR! Duplicate card found on line "), num);
            else {
                strcat(strcpy(buffer, "ERROR! Invalid card found on line "), num);
            }
            emptyView("LD", buffer);
            deleteLinkedList(cardDeck);
            free(num); 
            return NULL;
        } else {
            appendCard(cardDeck, newCard);
            ++lineNum;
        }
    }

    if (cardDeck->size != 4 * SUIT_SIZE) {
        emptyView("LD", "ERROR! CardDeck doesn't match the deck size of 52");
        deleteLinkedList(cardDeck);
        return NULL;
    }

    return cardDeck;
}

void saveDeck(Linked_list *list, FILE *fptr) {
    char line[3];
    line[2] = '\n';
    struct ListCard *node = list->head;
    while (node != NULL) {
        struct ListCard *card = node;
        line[0] = card->value;
        line[1] = card->suit;
        fwrite(line, 1, sizeof(line), fptr);
        node = node->next;
    }
}