#include "view.h"

// Variable to store the maximum number of rows in the view
int max_num_of_rows;
// Constant representing the number of columns in the game
const int NUM_OF_COLUMNS = 7;

// Function to display an empty view
void emptyView(char lastCommand[], char message[]) {
    // Clear the view
    clearView();
    // Generate the columns
    generateColumns();
    int Fnum = 1;
    max_num_of_rows = 7;

    // Loop through rows
    for (int i = 1; i <= max_num_of_rows; i++) {
        // Print tabs for alignment with columns
        for (int j = 0; j < NUM_OF_COLUMNS; ++j) {
            printf("\t");
        }

        // Print foundation slots on odd rows
        if (i % 2 == 1 && i < 8) {
            printf("\t[]\tF%d\n", Fnum);
            Fnum++;
        } else {
            printf("\n");
        }
    }

    // Display input line and status message
    displayInputLine(lastCommand, message);
}

// Function to display the deck
void showDeck(Linked_list *deck_list, char command[], char statusMessage[]) {
    // Check if the deck is empty
    if (deck_list == NULL) {
        // Display empty view with message
        emptyView(command, "No deck of cards is loaded");
        return;
    }

    // Clear the view
    clearView();
    // Generate the columns
    generateColumns();

    int Fnum = 1;
    struct ListCard *current_card = deck_list->tail;
    char value, suit;
    // Calculate the maximum number of rows based on deck size
    max_num_of_rows = (int) deck_list->size / 7 + 1;
    max_num_of_rows = max_num_of_rows > 7 ? max_num_of_rows : 7;

    // Loop through rows
    for (int i = 1; i <= max_num_of_rows; i++) {
        // Loop through columns
        for (int j = 0; j < NUM_OF_COLUMNS; ++j) {
            // Check if current card exists
            if (current_card == NULL) {
                printf("\t");
                continue;
            }

            // Determine card value and suit to display
            if (strcasecmp("SW", command) == 0) {
                value = current_card->value;
                suit = current_card->suit;
            } else {
                suit = ']';
                value = '[';
            }

            // Print card value and suit
            printf("%c%c\t", value, suit);
            current_card = current_card->prev;
        }

        // Print foundation slots on odd rows
        if (i % 2 == 1 && i < 8) {
            printf("\t[]\tF%d\n", Fnum);
            Fnum++;
        } else {
            printf("\n");
        }
    }

    // Display input line and status message
    displayInputLine(command, statusMessage);
}

// Function to generate the play view
void generatePlayView(Linked_list *C_ptr[7], Linked_list *F_ptr[4], char lastCommand[], char message[]) {
    // Determine the maximum number of rows required
    max_num_of_rows = determineMaxRows(C_ptr);

    // Clear the view
    clearView();
    // Generate the columns
    generateColumns();

    Linked_list *current_column;
    struct ListCard *current_card;
    int foundNum = 1;
    char value, suit;

    // Loop through rows
    for (int i = 1; i <= max_num_of_rows; i++) {
        // Loop through columns
        for (int j = 0; j < NUM_OF_COLUMNS; ++j) {
            // Get the current column
            current_column = C_ptr[j];

            // Get the card at the current row
            if (current_column != NULL) {
                current_card = current_column->head;
                for (int k = 0; k < i - 1; ++k) {
                    if (current_card != NULL)
                        current_card = current_card->next;
                    else break;
                }
            }

            // Check if current card exists
            if (current_card == NULL || current_column == NULL) {
                printf("\t");
                continue;
            }

            // Determine card value and suit to display
            if (current_card->faceDown == false) {
                suit = current_card->suit;
                value = current_card->value;
            } else {
                suit = ']';
                value = '[';
            }

            // Print card value and suit
            printf("%c%c\t", value, suit);
        }

        // Print foundation slots on odd rows
        if (i % 2 == 1 && i < 8) {
            // Get the current foundation
            Linked_list *current_foundation = F_ptr[foundNum - 1];
            struct ListCard *foundation_Card = NULL;

            // Get the card in the foundation
            if (current_foundation->tail != NULL) {
                foundation_Card = current_foundation->tail;
            }

            // Print foundation card or empty slot
            if (foundation_Card == NULL) {
                printf("\t[]\tF%d\n", foundNum);
            } else {
                suit = foundation_Card->suit;
                value = foundation_Card->value;
                printf("\t%c%c\tF%d\n", value, suit, foundNum);
            }
            foundNum++;
        } else {
            printf("\n");
        }
    }

    // Display input line and status message
    displayInputLine(lastCommand, message);
}

// Function to determine the maximum number of rows needed to display the columns
int determineMaxRows(Linked_list *columns[]) {
    int maxNumOfRows = 0;
    for (int i = 0; i < NUM_OF_COLUMNS; ++i) {
        Linked_list *tmp = columns[i];
        if (tmp == NULL) continue;
        if (maxNumOfRows < tmp->size) maxNumOfRows = tmp->size;
    }
    return maxNumOfRows > 7 ? maxNumOfRows : 7;
}

// Function to generate column labels
void generateColumns() {
    for (int i = 1; i <= 7; ++i) {
        printf("C%d\t", i);
    }
    printf("\n\n");
}

// Function to display input line and status message
void displayInputLine(char lastCommand[], char message[]) {
    printf("\nLAST Command: %s \n", lastCommand);
    printf("Message: %s \n", message);
    printf("INPUT > ");
}

// Function to clear the view
void clearView() {
    printf("\n\n\n\n\n\n");

#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    system("clear");
#endif

#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#endif
}
