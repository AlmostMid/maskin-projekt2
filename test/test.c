#include "../src/Linkedlists.h"
#include "../src/gameCommands.h"

int main() {
    // Load deck test
    printf("Testing Load Deck (LD)\n");
    Linked_list *loadedDeck = LD("default", 2);

    if (loadedDeck == NULL) {
        printf("Load deck test failed!\n");
        return 1;
    } else {
        printf("Load deck test passed!\n");
    }

    // Split deck test
    printf("Testing Split Deck (SI)\n");
    Linked_list *splitDeck = SI(loadedDeck, 26); // Split the deck in half

    if (splitDeck == NULL || loadedDeck->size != 26 || splitDeck->size != 26) {
        printf("Split deck test failed!\n");
        return 1;
    } else {
        printf("Split deck test passed!\n");
    }

    // Shuffle deck test
    printf("Testing Shuffle Deck (SR)\n");
    Linked_list *shuffledDeck = SR(loadedDeck);

    if (shuffledDeck == NULL || loadedDeck->size != 0 || shuffledDeck->size != 52) {
        printf("Shuffle deck test failed!\n");
        return 1;
    } else {
        printf("Shuffle deck test passed!\n");
    }

    // Save deck test
    printf("Testing Save Deck (SD)\n");
    SD(shuffledDeck, "shuffled_deck");

    FILE *fptr = fopen("../resources/shuffled_deck.txt", "r");

    if (fptr == NULL) {
        printf("Save deck test failed!\n");
        return 1;
    } else {
        printf("Save deck test passed!\n");
    }

    fclose(fptr);

    // Play game moves test
    printf("Testing Play Game Moves (gameMoves)\n");
    Linked_list **column_lists = P(shuffledDeck);
    Linked_list **foundation_lists = P(shuffledDeck);

    // Example move: Move a card from column 1 to foundation 1
    char move[] = "C1 -> F1";

    if (!gameMoves(move, column_lists, foundation_lists)) {
        printf("Play game moves test failed!\n");
        return 1;
    } else {
        printf("Play game moves test passed!\n");
    }

    printf("All tests passed!\n");

    return 0;
}
