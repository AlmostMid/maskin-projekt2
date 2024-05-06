#include "gameCommands.h"

// Function to load a deck from a specified file or a default file if no arguments are provided
Linked_list *LD(char arg[], int numOfInputs) {
    char filepath[256]; // Buffer for file path
    Linked_list *deckLoader; // Pointer to the linked list holding the deck

    if (numOfInputs == 2) {
        // Construct file path from input argument
        strncpy(filepath, "../resources/", 256);
        strcat(filepath, arg);
        strcat(filepath, ".txt");

        FILE *file = fopen(filepath, "r"); // Open file for reading

        /** If file exists */
        if (file != NULL) {
            deckLoader = loadDeck(file); // Load deck from file
            if (deckLoader != NULL) {
                showDeck(deckLoader, "LD", "OK"); // Display the deck
                fclose(file); // Close the file
            } else {
                fclose(file); // Close file and return NULL if deck failed to load
                return NULL;
            }
        } else {
            emptyView("LD", "Loaded file does not exist"); // Show error if file does not exist
            return NULL;
        }

    } else {
        // Load default deck if no specific file provided
        FILE *defaultDeck = fopen("../resources/default.txt", "r");
        if (defaultDeck != NULL) {
            deckLoader = loadDeck(defaultDeck);
            if (deckLoader != NULL) {
                showDeck(deckLoader, "LD", "unshuffled deck loaded");
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

    return deckLoader; // Return the loaded deck
}

Linked_list *SI(Linked_list *firstPile, int split) {
    if (split <= 0 || split >= firstPile->size) {
        emptyView("SI", "ERROR! Split index out of bounds.");
        return NULL;
    }

    // Create a new linked list for the second half
    Linked_list *secondPile = createLinkedList();
    struct ListCard *current = firstPile->head;

    // Move to the split point
    for (int i = 0; i < split - 1; i++) {
        current = current->next;
    }

    // Set the head of the second pile
    secondPile->head = current->next;
    current->next = NULL;  // Split the first pile

    // Recalculate the sizes
    secondPile->size = firstPile->size - split;
    firstPile->size = split;

    return secondPile;
}


Linked_list *SR(Linked_list *unshuffledPile) {
    int count = unshuffledPile->size;
    struct ListCard **cardArray = malloc(sizeof(struct ListCard *) * count);
    struct ListCard *current = unshuffledPile->head;

    // Fill the array with pointers to the cards
    for (int i = 0; i < count; i++) {
        cardArray[i] = current;
        current = current->next;
    }

    // Fisher-Yates shuffle algorithm
    for (int i = count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        struct ListCard *temp = cardArray[i];
        cardArray[i] = cardArray[j];
        cardArray[j] = temp;
    }

    // Re-link the cards
    for (int i = 0; i < count - 1; i++) {
        cardArray[i]->next = cardArray[i + 1];
    }
    cardArray[count - 1]->next = NULL;  // Last card points to NULL

    // Set the new head and tail
    unshuffledPile->head = cardArray[0];
    unshuffledPile->tail = cardArray[count - 1];

    free(cardArray);
    return unshuffledPile;
}


// Function to save a deck to a specified file
void SD(Linked_list *list, char arg[]) {
    char filepath[256];
    strncpy(filepath, "../resources/", 256);
    strcat(filepath, arg);
    strcat(filepath, ".txt");
    FILE *fptr = fopen(filepath, "w");
    saveDeck(list, fptr);
    fclose(fptr);
}

// Function to distribute cards into columns for gameplay
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
    for (int j = 1, i = 1; j <= 7; ++j) {
        if (current_card == NULL) break;

        current_card->faceDown = false;
        // Logic for distributing cards to columns with face down settings
        // More distribution logic...

        appendCard(toDeck, *current_card);
        current_card = current_card->prev;
        if (j >= 7) {
            i++;
            j = 0;
        }
    }

    Linked_list **C_ptr = malloc(sizeof(Linked_list *) * 7); // Array of pointers to columns
    C_ptr[0] = C1; // Assign columns to array
    // More assignments...

    return C_ptr;
}

// Function to handle game moves like moving cards between columns or to foundations
bool gameMoves(char buf[], Linked_list **column_lists, Linked_list **foundation_lists) {
    char delimiters[] = "-> :\n";
    char *token;
    char command[256];
    strcpy(command, buf);

    char gameMove[3][3] = {0};

    token = strtok(buf, delimiters);

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
    if (gameMove[2][0] == '\0' && gameMove[2][1] == '\0' && gameMove[2][2] == '\0') {
        gameMove[2][0] = gameMove[1][0];
        gameMove[2][1] = gameMove[1][1];
        gameMove[2][2] = gameMove[1][2];
        fromCardExcists = false;
    }

    int from = gameMove[0][1] - 49;
    int to = gameMove[2][1] - 49;

    char fromCard[2];
    bool toFoundation = false;

    Linked_list *fromList = NULL;
    Linked_list *toList = NULL;
    if ((gameMove[0][0] == 'C' || gameMove[0][0] == 'c') &&
        strcmp(&gameMove[0][0], &gameMove[2][0]) != 0) {
        fromList = column_lists[from];
        if (fromCardExcists) {
            fromCard[0] = gameMove[1][0];
            fromCard[1] = gameMove[1][1];
        } else if (fromList->tail != NULL) {
            fromCard[0] = fromList->tail->value;
            fromCard[1] = fromList->tail->suit;
        } else {
            generatePlayView(column_lists, foundation_lists, command, "Invalid <FROM> command.");
            return false;
        }
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
    struct ListCard *nodeFrom = findNodeFromCard(fromList, toupper(fromCard[0]), toupper(fromCard[1]));

    if (nodeFrom->prev != NULL && nodeFrom->prev->faceDown) {
        nodeFrom->prev->faceDown = false;
    }

    if (nodeFrom == NULL) {
        generatePlayView(column_lists, foundation_lists, command, "Card not found");
        return false;
    }

    if (isMoveValid(nodeFrom, toList->tail, toFoundation) == false) {
        generatePlayView(column_lists, foundation_lists, command, "Invalid move");
        return false;
    }

    moveCardBetweenLists(fromList, nodeFrom, toList);

    generatePlayView(column_lists, foundation_lists, command, "OK");

}

// Function to check if a move is valid according to the game rules
bool isMoveValid(struct ListCard *from, struct ListCard *to, bool toFoundation) {
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
