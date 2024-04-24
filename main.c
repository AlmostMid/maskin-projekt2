#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "src/view.h"
#include "src/deck.h"
#include "src/gameCommands.h"
#include "src/Linkedlists.h"

bool startUpPhase(Linked_list **loadedDeck, bool *deckLoaded);

bool playPhase(Linked_list **loadedDeck);

bool checkIfWinner(Linked_list *foundations[]);

int main(void) {
#if defined(_WIN32) || defined(_WIN64)
    setbuf(stdout, 0);
#endif

    Linked_list *loadedDeck = createLinkedList();
    bool deckLoaded = false;
    bool gameRunning = true;
    // Start view
    emptyView("", "");

    while (gameRunning) {
        gameRunning = startUpPhase(&loadedDeck, &deckLoaded);

        if (gameRunning)
            gameRunning = playPhase(&loadedDeck);
    }

    return 0;
}

bool startUpPhase(Linked_list **loadedDeck, bool *deckLoaded) {
    char command[256] = {0}, arg[256] = {0}, buf[256] = {0};

    while ((strcasecmp("P", command) != 0)) {
        fgets(buf, sizeof(buf), stdin);
        int numOfInputs = sscanf(buf, "%s %s", command, arg);

        if (strcasecmp("LD", command) == 0) {
            Linked_list *result = LD(arg, numOfInputs);
            if (result != NULL) {
                deleteLinkedList(*loadedDeck);
                *loadedDeck = result;
                *deckLoaded = true;
            }

        } else if (*deckLoaded && strcasecmp("SW", command) == 0) {
            showDeck(*loadedDeck, "SW", "OK");
        } else if (*deckLoaded && strcasecmp("SI", command) == 0) {
            int split;

            if (numOfInputs == 1) {
                split = rand() % (((Linked_list *) *loadedDeck)->size - 1) + 1;
            } else {
                split = atoi(arg);
            }

            Linked_list *result = SI(*loadedDeck, split);
            if (result != NULL) {
                *loadedDeck = result;
                showDeck(*loadedDeck, "SI", "OK");
            }
        } else if (*deckLoaded && strcasecmp("SR", command) == 0) {
            *loadedDeck = SR(*loadedDeck);
            showDeck(*loadedDeck, "SR", "OK");
        } else if (*deckLoaded && strcasecmp("SD", command) == 0) {
            if (numOfInputs == 1) {
                SD(*loadedDeck, "cards");
            } else {
                SD(*loadedDeck, arg);
            }
            showDeck(*loadedDeck, "SD", "Deck has been saved.");

        } else if (strcasecmp("QQ", command) == 0) {
            puts("Ending Yukon...");
            return false;
        } else if (*deckLoaded && strcasecmp("P", command) == 0) {
            return true;
        } else {
            emptyView("", "Error! Invalid command");
        }
    }
}

bool playPhase(Linked_list **loadedDeck) {
    Linked_list **column_lists = P(*loadedDeck);
    Linked_list *foundation_lists[4] = {createLinkedList(), createLinkedList(),
                                        createLinkedList(), createLinkedList()};
    generatePlayView(column_lists, foundation_lists, "P", "OK");

    char command[256] = {0}, arg[256] = {0}, buf[256] = {0};
    while ((strcasecmp("Q", command) != 0)) {
        fgets(buf, sizeof(buf), stdin);
        int numOfInputs = sscanf(buf, "%s %s", command, arg);

        // Check if the command contains "->" using strchr
        char *arrowPtr = strchr(buf, '-');
        if (arrowPtr != NULL && (strcasecmp("C", command) == 0 || strcasecmp("F", command) == 0)) {
            *arrowPtr = '\0'; // Null-terminate at the arrow position
            sscanf(buf, "%s %s %s", command, arg, arg + 3);
            strcat(arg, "->");
        }

        if (strcasecmp("LD", command) == 0 || strcasecmp("SW", command) == 0
            || strcasecmp("SI", command) == 0 || strcasecmp("SR", command) == 0
            || strcasecmp("SD", command) == 0) {
            generatePlayView(column_lists, foundation_lists,
                             command, "ERROR! Command not available in the PLAY phase");
        } else if (strcasecmp("QQ", command) == 0) {
            puts("Ending Yukon...");
            return false;
        } else if (strcasecmp("Q", command) == 0) {
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
            gameMoves(buf, column_lists, foundation_lists);
        }

        bool winner = checkIfWinner(foundation_lists);
        if (winner) {
            emptyView("", "Game Won");
            return !winner;
        }
    }
}

bool checkIfWinner(Linked_list *foundations[]) {
    int filledFoundations = 0;
    for (int i = 0; i < 4; i++) {
        if (foundations[i]->size == SUIT_SIZE)
            filledFoundations++;
    }

    return filledFoundations == 4;
}