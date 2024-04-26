#include "gameCommands.h"

Linked_list *LD(char arg[], int numOfInputs) {
    char filepath[256];
    Linked_list *loadedDeck;

    /** If filepath is not empty */
    if (numOfInputs == 2) {
        strncpy(filepath, "../resources/", 256);
        strcat(filepath, arg);
        strcat(filepath, ".txt");

        FILE *file = fopen(filepath, "r");

        /** If file exists */
        if (file != NULL) {
            loadedDeck = loadDeck(file);
            if (loadedDeck != NULL) {
                showDeck(loadedDeck, "LD", "OK");
                fclose(file);
            } else {
                fclose(file);
                return NULL;
            }
        } else {
            emptyView("LD", "Loaded file does not exists");
            return NULL;
        }

        /** If no filepath is given, load unshuffled deck */
    } else {
        FILE *defaultDeck = fopen("../resources/default.txt", "r");
        if (defaultDeck != NULL) {
            loadedDeck = loadDeck(defaultDeck);

            if (loadedDeck != NULL) {
                showDeck(loadedDeck, "LD", "unshuffled deck loaded");
                fclose(defaultDeck);
            } else {
                fclose(defaultDeck);
                return NULL;
            }
        } else {
            emptyView("LD", "Default deck does not exist");
            return NULL;
        }
    }

    return loadedDeck;
}

Linked_list *SI(Linked_list *firstPile, int split) {
    if (split <= 0) {
       emptyView("SI", "Not able to split on 0 or non-numbers.");
        return NULL;
    } else if (split >= firstPile->size) {
        emptyView("SI", "Number bigger than the number of cards in the deck.");
        return NULL;
    }

    // Splits the deck into two decks, which should be interleaved
    Linked_list *secondPile = createLinkedList();

    struct ListCard *card = firstPile->head;
    for (int i = 0; i < split; ++i) {
        card = card->next;
    }

    moveCardFromOneLinkedListToAnother(firstPile, card, secondPile);

    // Interleaves the two decks into a shuffled pile
    Linked_list *shuffledPile = createLinkedList();
    while (firstPile->size > 0 || secondPile->size > 0) {
        if (firstPile->size > 0) {
            prependCard(shuffledPile, *firstPile->tail);
            removeNode(firstPile);
        }
        if (secondPile->size > 0) {
            prependCard(shuffledPile, *secondPile->tail);
            removeNode(secondPile);
        }
    }

    deleteLinkedList(firstPile);
    deleteLinkedList(secondPile);

    return shuffledPile;
}

Linked_list *SR(Linked_list *unshuffledPile) {
    Linked_list *shuffledPile = createLinkedList();
    srand(time(0));

    struct ListCard *node = unshuffledPile->tail;
    while (node != NULL) {
        // Calculate a random placement within the ranges of the shuffled piles size.
        // Add with 1 to avoid zero division error.
        int placement = rand() % (shuffledPile->size + 1);

        // Finds the placement in the shuffled pile, so we know where to insert
        struct ListCard *shuffNode = shuffledPile->head;
        for (int i = 0; i < placement - 1; i++) {
            shuffNode = shuffNode->next;
        }

        // Randomize if the card is placed before or after the chosen placement
        int before = rand() % 2;
        insertNode(shuffledPile, node, shuffNode, before);

        node = node->prev;
    }

    // Avoid memory leak of deck piles
    deleteLinkedList(unshuffledPile);
    return shuffledPile;
}

void SD(Linked_list *list, char arg[]) {
    char filepath[256];
    strncpy(filepath, "../resources/", 256);
    strcat(filepath, arg);
    strcat(filepath, ".txt");
    FILE *fptr = fopen(filepath, "w");
    saveDeck(list, fptr);
    fclose(fptr);
}
Linked_list **P(Linked_list *loadedDeck) {
    Linked_list *C1 = createLinkedList();
    Linked_list *C2 = createLinkedList();
    Linked_list *C3 = createLinkedList();
    Linked_list *C4 = createLinkedList();
    Linked_list *C5 = createLinkedList();
    Linked_list *C6 = createLinkedList();
    Linked_list *C7 = createLinkedList();
    struct ListCard *current_card = loadedDeck->tail;

    Linked_list *toDeck = NULL;
    // Columns
    for (int j = 1, i = 1; j <= 7; ++j) {
        if (current_card == NULL) break;

        current_card->faceDown = false;
        if (j == 1 && i <= 1) {
            toDeck = C1;
        } else if (j == 2 && i <= 6) {
            toDeck = C2;
            if (i <= 1)
                current_card->faceDown = true;
        } else if (j == 3 && i <= 7) {
            toDeck = C3;
            if (i <= 2) current_card->faceDown = true;
        } else if (j == 4 && i <= 8) {
            toDeck = C4;
            if (i <= 3) current_card->faceDown = true;
        } else if (j == 5 && i <= 9) {
            toDeck = C5;
            if (i <= 4) current_card->faceDown = true;
        } else if (j == 6 && i <= 10) {
            toDeck = C6;
            if (i <= 5) current_card->faceDown = true;
        } else if (j == 7 && i <= 11) {
            toDeck = C7;
            if (i <= 6) current_card->faceDown = true;
        } else continue;

        appendCard(toDeck, *current_card);
        current_card = current_card->prev;
        if (j >= 7) {
            // Determines faceUp or faceDown
            i++;
            j = 0;
        }
    }

    Linked_list **C_ptr = malloc(sizeof(Linked_list *) * 7);
    C_ptr[0] = C1;
    C_ptr[1] = C2;
    C_ptr[2] = C3;
    C_ptr[3] = C4;
    C_ptr[4] = C5;
    C_ptr[5] = C6;
    C_ptr[6] = C7;
    return C_ptr;
}

bool gameMoves(char buf[], Linked_list **column_lists, Linked_list **foundation_lists) {
    char delimiters[] = "-> :\n";
    char *token;
    char command[256];
    strcpy(command, buf);

    char gameMove[3][3] = {0};

    token = strtok(buf, delimiters);

    // Load gameMove in 2D array with string tokens
    int i = 0;
    while (token != NULL && i < 3) {
        int j = 0;
        while (j < 2) {
            gameMove[i][j] = token[j];
            ++j;
        }
        token = strtok(NULL, delimiters);
        ++i;
    }

    bool fromCardExcists = true;
    // If no from Card is present, recalibrate game move positions in array
    if (gameMove[2][0] == '\0' && gameMove[2][1] == '\0' && gameMove[2][2] == '\0') {
        gameMove[2][0] = gameMove[1][0];
        gameMove[2][1] = gameMove[1][1];
        gameMove[2][2] = gameMove[1][2];
        fromCardExcists = false;
    }

    // Get columns/foundation numbers converted from ASCII value
    int from = gameMove[0][1] - 49;
    int to = gameMove[2][1] - 49;

    // We need fromCard later to get the optional card. 0. value : 1. suit
    char fromCard[2];
    bool toFoundation = false;

    Linked_list *fromList = NULL;
    Linked_list *toList = NULL;
    // Check if <FROM> is Column (C) or Foundation (F).
    if ((gameMove[0][0] == 'C' || gameMove[0][0] == 'c') &&
        strcmp(&gameMove[0][0], &gameMove[2][0]) != 0) {
        // Set <FROM> list
        fromList = column_lists[from];
        // Set <FROM> card
        if (fromCardExcists) {
            fromCard[0] = gameMove[1][0];
            fromCard[1] = gameMove[1][1];
        } else if (fromList->tail != NULL) {
            // No from card. Set tail as <FROM> Card
            fromCard[0] = fromList->tail->value;
            fromCard[1] = fromList->tail->suit;
        } else {
            generatePlayView(column_lists, foundation_lists, command, "Invalid <FROM> command.");
            return false;
        }
        // If <FROM> is C, then we check <TO> for either C or F. If none, then we can error handle
        if (gameMove[2][0] == 'C' || gameMove[2][0] == 'c') toList = column_lists[to];
        else if (gameMove[2][0] == 'F' || gameMove[2][0] == 'f') {
            toList = foundation_lists[to];
            toFoundation = true;
        } else {
            generatePlayView(column_lists, foundation_lists, command, "Invalid <TO> command.");
            return false;
        }
    } else if ((gameMove[0][0] == 'F' || gameMove[0][0] == 'f') &&
               (gameMove[1][0] == 'C' || gameMove[1][0] == 'c') && foundation_lists[from]->tail != NULL) {
        // If <FROM> is F, then we can only move to a C. We use the top on F as the card from.
        int toColumn = gameMove[1][1] - 49;
        fromList = foundation_lists[from];
        toList = column_lists[toColumn];
        struct ListCard *tempCard = fromList->tail;
        fromCard[0] = tempCard->value;
        fromCard[1] = tempCard->suit;
    } else {
        generatePlayView(column_lists, foundation_lists, command, "Is not a valid move");
        return false;
    }
    // Search for the node in the specified column
    struct ListCard *nodeFrom = findNodeFromCard(fromList, toupper(fromCard[0]), toupper(fromCard[1]));

    if (nodeFrom->prev != NULL && nodeFrom->prev->faceDown) {
        nodeFrom->prev->faceDown = false;
    }

    // Check for card not found
    if (nodeFrom == NULL) {
        generatePlayView(column_lists, foundation_lists, command, "Card not found");
        return false;
    }

    if (moveValidation(nodeFrom, toList->tail, toFoundation) == false) {
        generatePlayView(column_lists, foundation_lists, command, "Invalid move");
        return false;
    }

    // Move the card to the new column
    moveCardFromOneLinkedListToAnother(fromList, nodeFrom, toList);

    // Show deck
    generatePlayView(column_lists, foundation_lists, command, "OK");

}

bool moveValidation(struct ListCard *from, struct ListCard *to, bool toFoundation) {
    bool result = false;
    // If moving to empty foundation
    if (to == NULL && from->value == 'A' && from->next == NULL && toFoundation) return true;
    if (to == NULL && from->value == 'K' && !toFoundation) return true;
    if (to == NULL) return false;

    int fromValue = convertASCII(from->value) + 1;
    int toValue = convertASCII(to->value) + 1;

    int diff = toValue - fromValue;

    if (toFoundation) {
        if (diff == -1 && from->suit == to->suit) result = true;
    } else if (diff == 1 && from->suit != to->suit) result = true;
    return result;
}
