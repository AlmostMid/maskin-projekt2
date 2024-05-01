#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "view.h"
#include "deck.h"
#include "gameCommands.h"

// Function prototypes
bool startUpPhase(Linked_list **loadedDeck, bool *deckLoaded);
bool playPhase(Linked_list **loadedDeck);
bool checkWinner(Linked_list *foundations[]);

// Function to run the Yukon game
void runYukonGame() {
    // Initialize variables
    Linked_list *loadedDeck = createLinkedList();
    bool deckLoaded = false;
    bool gameRunning = true;
    // Start view
    emptyView("", "");

    // Main game loop
    while (gameRunning) {
        // Execute startup phase
        gameRunning = startUpPhase(&loadedDeck, &deckLoaded);

        // If startup phase successful, execute play phase
        if (gameRunning)
            gameRunning = playPhase(&loadedDeck);
    }
}

// Function to handle the startup phase of the game
bool startUpPhase(Linked_list **loadedDeck, bool *deckLoaded) {
    // Initialize variables
    char command[256] = {0}, arg[256] = {0}, buf[256] = {0};

    // Loop until 'P' command is received
    while ((strcasecmp("P", command) != 0)) {
        // Read user input
        fgets(buf, sizeof(buf), stdin);
        // Parse command and argument
        int numOfInputs = sscanf(buf, "%s %s", command, arg);

        // Process different commands during startup phase
        if (strcasecmp("LD", command) == 0) {
            // Load deck
            Linked_list *result = LD(arg, numOfInputs);
            if (result != NULL) {
                deleteLinkedList(*loadedDeck); // Delete previously loaded deck if any
                *loadedDeck = result; // Set the new loaded deck
                *deckLoaded = true; // Set deck loaded flag
            }

        } else if (*deckLoaded && strcasecmp("SW", command) == 0) {
            // Show deck
            showDeck(*loadedDeck, "SW", "OK");

        } else if (*deckLoaded && strcasecmp("SI", command) == 0) {
            // Split deck
            int split;
            if (numOfInputs == 1) {
                split = rand() % (((Linked_list *) *loadedDeck)->size - 1) + 1; // Random split if no value provided
            } else {
                split = atoi(arg); // Convert argument to integer
            }
            Linked_list *result = SI(*loadedDeck, split);
            if (result != NULL) {
                *loadedDeck = result; // Set the new split deck
                showDeck(*loadedDeck, "SI", "OK"); // Show split deck
            }

        } else if (*deckLoaded && strcasecmp("SR", command) == 0) {
            // Shuffle deck
            *loadedDeck = SR(*loadedDeck);
            showDeck(*loadedDeck, "SR", "OK"); // Show shuffled deck

        } else if (*deckLoaded && strcasecmp("SD", command) == 0) {
            // Save deck to file
            if (numOfInputs == 1) {
                SD(*loadedDeck, "cards");
            } else {
                SD(*loadedDeck, arg);
            }
            showDeck(*loadedDeck, "SD", "Deck has been saved."); // Show confirmation message

        } else if (strcasecmp("QQ", command) == 0) {
            // Quit the game
            puts("Quitting Yukon...");
            return false;

        } else if (*deckLoaded && strcasecmp("P", command) == 0) {
            // Start play phase if deck is loaded
            return true;

        } else {
            // Invalid command
            emptyView("", "Invalid command");
        }
    }
}

// Function to handle the play phase of the game
bool playPhase(Linked_list **loadedDeck) {
    // Initialize variables
    Linked_list **column_lists = P(*loadedDeck); // Initialize columns with cards
    Linked_list *foundation_lists[4] = {createLinkedList(), createLinkedList(),
                                        createLinkedList(), createLinkedList()}; // Initialize foundation lists

    // Generate initial play view
    generatePlayView(column_lists, foundation_lists, "P", "OK");

    // Initialize variables
    char command[256] = {0}, arg[256] = {0}, buf[256] = {0};

    // Loop until 'Q' command is received
    while ((strcasecmp("Q", command) != 0)) {
        // Read user input
        fgets(buf, sizeof(buf), stdin);
        // Parse command and argument
        int numOfInputs = sscanf(buf, "%s %s", command, arg);

        // Handle commands with an arrow ('->') separator
        char *arrowPtr = strchr(buf, '-');
        if (arrowPtr != NULL && (strcasecmp("C", command) == 0 || strcasecmp("F", command) == 0)) {
            *arrowPtr = '\0'; // Null-terminate at the arrow position
            sscanf(buf, "%s %s %s", command, arg, arg + 3);
            strcat(arg, "->"); // Concatenate the arrow back to the argument
        }

        // Process different commands during play phase
        if (strcasecmp("LD", command) == 0 || strcasecmp("SW", command) == 0
            || strcasecmp("SI", command) == 0 || strcasecmp("SR", command) == 0
            || strcasecmp("SD", command) == 0) {
            // Command not available in the PLAY phase
            generatePlayView(column_lists, foundation_lists,
                             command, "Command not available in the PLAY phase");

        } else if (strcasecmp("QQ", command) == 0) {
            // Quit the game
            puts("Ending Yukon...");
            return false;

        } else if (strcasecmp("Q", command) == 0) {
            // Clean up memory and return to startup phase
            for (int i = 0; i < 7; ++i) {
                deleteLinkedList(column_lists[i]);
            }
            free(column_lists);
            for (int i = 0; i < 4; ++i) {
                deleteLinkedList(foundation_lists[i]);
            }
            emptyView("Q", "OK. You are now in the STARTUP Phase");
            return true;

        } else {
            // Process game moves
            gameMoves(buf, column_lists, foundation_lists);
        }

        // Check for winner
        bool winner = checkWinner(foundation_lists);
        if (winner) {
            // Show winner message
            emptyView("", "Game Won");
            return !winner; // Exit the game
        }
    }
}

// Function to check if a player has won the game
bool checkWinner(Linked_list *foundations[]) {
    // Count filled foundation lists
    int filledFoundations = 0;
    for (int i = 0; i < 4; i++) {
        if (foundations[i]->size == SUIT_SIZE)
            filledFoundations++;
    }

    return filledFoundations == 4; // Return true if all foundations are filled
}
