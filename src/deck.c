// Include necessary headers for card deck and view operations
#include "deck.h"
#include "view.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Initialize arrays for each suit in a standard deck of cards
Card clubs[SUIT_SIZE];
Card diamonds[SUIT_SIZE];
Card hearts[SUIT_SIZE];
Card spades[SUIT_SIZE];

// Array of pointers to each suit array
Card *deck[4] = {clubs, diamonds, hearts, spades};

// Function to initialize each suit with cards A, 2-10, J, Q, K
void fillSuit() {
    char suits[4] = {'C', 'D', 'H', 'S'};  // Array to hold suit characters

    for (int i = 0; i < 4; i++) {
        // Set up Ace
        deck[i][0].value = 'A';
        deck[i][0].suit = suits[i];
        deck[i][0].exists = false;  // Initial state is not in play

        // Set up cards 2 to 10
        for (int j = 1; j < 10; j++) {
            deck[i][j].value = '0' + (j + 1);
            deck[i][j].suit = suits[i];
            deck[i][j].exists = false;
        }

        // Set up Ten, Jack, Queen, King
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

// Function to check if a card is valid and update its existence status
int checkCard(struct ListCard *deck_card) {
    int suit;
    // Determine the suit index
    switch (deck_card->suit) {
        case 'C': suit = 0; break;
        case 'D': suit = 1; break;
        case 'H': suit = 2; break;
        case 'S': suit = 3; break;
        default: return 2; // Invalid suit
    }

    // Convert card value from ASCII to index
    int value = convertASCII(deck_card->value);
    if (value == -1) return 2; // Invalid value

    // Check existence and update if necessary
    if (!deck[suit][value].exists) {
        deck[suit][value].exists = true;
        deck_card->exists = true;
        return 0;  // Card successfully added
    } else return 1; // Duplicate card found
}

// Convert card ASCII characters to index positions
int convertASCII(char valueOfCard) {
    int value;
    if (valueOfCard == 'A' || valueOfCard == 'a') {
        value = 0;  // Ace
    } else if (valueOfCard >= '1' && valueOfCard <= '9') {
        value = valueOfCard - '0' - 1;  // Numeric cards 2-9
    } else {
        switch (valueOfCard) {
            case 'T': value = 9; break;   // Ten
            case 'J': value = 10; break;  // Jack
            case 'Q': value = 11; break;  // Queen
            case 'K': value = 12; break;  // King
            default: return -1;  // Invalid character
        }
    }
    return value;
}

// Function to write the current deck to a file
void saveDeck(Linked_list *list, FILE *fptr) {
    char line[3];
    line[2] = '\n';
    struct ListCard *node = list->head;
    while (node != NULL) {
        line[0] = node->value;
        line[1] = node->suit;
        fwrite(line, 1, sizeof(line), fptr);  // Write card to file
        node = node->next;
    }
}

// Function to load a deck from a file, check validity, and handle errors
Linked_list *loadDeck(FILE *fptr) {
    fillSuit();  // Reset deck status
    char line[4];
    Linked_list *cardDeck = createLinkedList();

    int lineNum = 1;
    while (fgets(line, sizeof(line), fptr) != NULL) {
        line[strcspn(line, "\n")] = 0;  // Remove newline character

        struct ListCard newCard;
        newCard.value = line[0];
        newCard.suit = line[1];

        char buffer[40];
        char num[10];
        int check = checkCard(&newCard);

        // Handle error messages and cleanup
        if (check != 0 && sprintf(num, "%d", lineNum) != -1) {
            if (check == 1) {
                strcat(strcpy(buffer, "Duplicate card found on line "), num);
            } else {
                strcat(strcpy(buffer, "Invalid card found on line "), num);
            }
            emptyView("LD", buffer);
            deleteLinkedList(cardDeck);
            return NULL;
        } else {
            appendCard(cardDeck, newCard);
            ++lineNum;
        }
    }

    // Ensure deck has exactly 52 cards
    if (cardDeck->size != 4 * SUIT_SIZE) {
        emptyView("LD", "Deck doesn't match the size of 52");
        deleteLinkedList(cardDeck);
        return NULL;
    }

    return cardDeck;
}
