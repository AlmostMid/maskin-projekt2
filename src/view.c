#include "view.h"

// If a deck of more than (7 columns * 8 rows) = 56 cards is desired, increase this constant
int max_num_of_rows;
const int NUM_OF_COLUMNS = 7;

void emptyView(char lastCommand[], char message[]) {
    clearView();
    generateColumns();
    int Fnum = 1;
    max_num_of_rows = 7;

    for (int i = 1; i <= max_num_of_rows; i++) {
        for (int j = 0; j < NUM_OF_COLUMNS; ++j) {
            printf("\t");
        }

        if (i % 2 == 1 && i < 8) {
            printf("\t[]\tF%d\n", Fnum);
            Fnum++;
        }
        else {
            printf("\n");
        }
    }

    printCC(lastCommand, message);
}

void showDeck(Linked_list *deck_list, char command[], char statusMessage[]) {
    if (deck_list == NULL) {
        emptyView(command, "No deck of cards is loaded");
        return;
    }

    clearView();
    generateColumns();

    int Fnum = 1;
    struct ListCard *current_card = deck_list->tail;
    char value, suit;
    max_num_of_rows = (int) deck_list->size / 7 + 1;
    max_num_of_rows = max_num_of_rows > 7 ? max_num_of_rows : 7;


    for (int i = 1; i <= max_num_of_rows; i++) {

        for (int j = 0; j < NUM_OF_COLUMNS; ++j) {
            if (current_card == NULL) {
                printf("\t");
                continue;
            }
            if (strcasecmp("SW", command) == 0) {
                value = current_card->value;
                suit = current_card->suit;
            }
            else {
                value = '[';
                suit = ']';
            }

            printf("%c%c\t", value, suit);
            current_card = current_card->prev;
        }

        if (i % 2 == 1 && i < 8) {
            printf("\t[]\tF%d\n", Fnum);
            Fnum++;
        }
        else {
            printf("\n");
        }
    }
    printCC(command, statusMessage);
}

void generatePlayView(Linked_list *C_ptr[7], Linked_list *F_ptr[4], char lastCommand[], char message[]) {
    max_num_of_rows = calculateMaxRows(C_ptr);

    clearView();
    generateColumns();

    Linked_list *current_column;
    struct ListCard *current_card;
    int F_num = 1;
    char value, suit;

    for (int i = 1; i <= max_num_of_rows; i++) {

        for (int j = 0; j < NUM_OF_COLUMNS; ++j) {
            current_column = C_ptr[j];
            if (current_column != NULL) {
                current_card = current_column->head;
                for (int k = 0; k < i - 1; ++k) {
                    if (current_card != NULL)
                        current_card = current_card->next;
                    else break;
                }
            }

            if (current_card == NULL || current_column == NULL) {
                printf("\t");
                continue;
            }

            if (current_card->faceDown == false) {
                value = current_card->value;
                suit = current_card->suit;
            }
            else {
                value = '[';
                suit = ']';
            }

            printf("%c%c\t", value, suit);
        }


        Linked_list *current_foundation;
        struct ListCard *foundation_Card = NULL;
        if (i % 2 == 1 && i < 8) {
            current_foundation = F_ptr[F_num - 1];


            if (current_foundation->tail != NULL) {
                foundation_Card = current_foundation->tail;
            }

            if (foundation_Card == NULL) {
                printf("\t[]\tF%d\n", F_num);
                F_num++;
                continue;
            }
            else {
                value = foundation_Card->value;
                suit = foundation_Card->suit;
                printf("\t%c%c\tF%d\n", value, suit, F_num);
            }
            F_num++;
        }
        else {
            printf("\n");
        }
    }
    printCC(lastCommand, message);
}

int calculateMaxRows(Linked_list *columns[NUM_OF_COLUMNS]) {
    int maxNumOfRows = 0;
    for (int i = 0; i < NUM_OF_COLUMNS; ++i) {
        Linked_list *tmp = columns[i];
        if (tmp == NULL) continue;
        if (maxNumOfRows < tmp->size) maxNumOfRows = tmp->size;
    }
    return maxNumOfRows > 7 ? maxNumOfRows : 7;
}

void generateColumns() {
    for (int i = 1; i <= 7; ++i) {
        printf("C%d\t", i);
    }
    printf("\n\n");
}

void printCC(char lastCommand[], char message[]) {
    printf("\nLAST Command: %s \n", lastCommand);
    printf("Message: %s \n", message);
    printf("INPUT > ");
}

void clearView() {
    printf("\n\n\n\n\n\n");

#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    system("clear");
#endif

#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#endif
}