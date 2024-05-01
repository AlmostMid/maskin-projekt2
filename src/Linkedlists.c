#include "Linkedlists.h"

// Function to create a new empty linked list
Linked_list *createLinkedList() {
    Linked_list *newLinkedList;
    newLinkedList = (Linked_list *) malloc(sizeof(Linked_list)); // Allocate memory for the new linked list

    // Initialize the list properties
    newLinkedList->size = 0;
    newLinkedList->head = NULL;
    newLinkedList->tail = NULL;

    return newLinkedList; // Return the newly created linked list
}

// Function to append a card to the end of the list
void appendCard(Linked_list *list, struct ListCard card) {
    struct ListCard *newCard = (struct ListCard *) malloc(sizeof(struct ListCard)); // Allocate memory for the new card

    // Copy card data
    newCard->suit = card.suit;
    newCard->value = card.value;
    newCard->existsInGame = card.existsInGame;
    newCard->faceDown = card.faceDown;

    // If list is empty, initialize head and tail
    if (list->size == 0) {
        newCard->next = NULL;
        newCard->prev = NULL;
        list->head = newCard;
        list->tail = newCard;
    } else {
        // Otherwise, add to the end and update links
        newCard->prev = list->tail;
        list->tail->next = newCard;
        newCard->next = NULL;
        list->tail = newCard;
    }

    list->size++; // Increment the size of the list
}

// Function to prepend a card to the beginning of the list
void prependCard(Linked_list *list, struct ListCard card) {
    struct ListCard *newCard = (struct ListCard *) malloc(sizeof(struct ListCard)); // Allocate memory for the new card

    // Copy card data
    newCard->suit = card.suit;
    newCard->value = card.value;
    newCard->existsInGame = card.existsInGame;
    newCard->faceDown = card.faceDown;

    // If list is empty, initialize head and tail
    if (list->size == 0) {
        newCard->next = NULL;
        newCard->prev = NULL;
        list->head = newCard;
        list->tail = newCard;
    } else {
        // Otherwise, insert at the beginning and update links
        newCard->next = list->head;
        list->head->prev = newCard;
        newCard->prev = NULL;
        list->head = newCard;
    }

    list->size++; // Increment the size of the list
}

// Function to insert a node into the list at a specified location
void insertNode(Linked_list *list, struct ListCard *nodeToInsert, struct ListCard *previousNode, bool insertBefore) {
    struct ListCard *nodeCopy = (struct ListCard *) malloc(sizeof(struct ListCard)); // Allocate memory for the new card
    *nodeCopy = *nodeToInsert; // Copy node data

    // Insert before the specified node
    if (insertBefore && list->size > 0) {
        if (previousNode->prev != NULL) {
            // Node is not the first, insert in middle
            nodeCopy->next = previousNode;
            nodeCopy->prev = previousNode->prev;
            previousNode->prev->next = nodeCopy;
            previousNode->prev = nodeCopy;
        } else {
            // Node is the first, use prepend function
            prependCard(list, *nodeToInsert);
            free(nodeCopy);
            list->size--; // Adjust size since prepend increments it
        }
    } else {
        // Insert after the specified node
        if (list->size > 0 && previousNode->next != NULL) {
            nodeCopy->next = previousNode->next;
            nodeCopy->prev = previousNode;
            previousNode->next->prev = nodeCopy;
            previousNode->next = nodeCopy;
        } else {
            // Append if it's the last node
            appendCard(list, *nodeToInsert);
            free(nodeCopy);
            list->size--; // Adjust size since append increments it
        }
    }

    list->size++; // Increment list size
}

// Function to remove the last node from the list
void removeNode(Linked_list *list) {
    if (list->size == 0) {
        return; // No operation if the list is empty
    } else {
        struct ListCard *card = list->tail; // Start at the tail
        if (card->prev != NULL) {
            struct ListCard *prevNode = card->prev;
            prevNode->next = NULL;
            list->tail = prevNode;
        } else {
            // Only one card in the list
            list->tail = NULL;
            list->head = NULL;
        }

        free(card); // Free the removed card
        list->size--; // Decrement list size
    }
}

// Function to move a chain of cards between two lists
bool moveCardBetweenLists(Linked_list *from, struct ListCard *cardFrom, Linked_list *to) {
    bool result = false;
    struct ListCard *prevNode = to->tail;

    // Calculate how many cards are being moved
    int cardsMoved = 1;
    struct ListCard *lastMovedCard = cardFrom;
    while (lastMovedCard->next != NULL) {
        cardsMoved++;
        lastMovedCard = lastMovedCard->next;
    }

    // If card is valid, detach from 'from' list and attach to 'to' list
    if (cardFrom != NULL) {
        if (cardFrom->prev != NULL)
            cardFrom->prev->next = NULL;

        from->tail = cardFrom->prev;
        from->size -= cardsMoved;

        if (from->size == 0) {
            from->head = NULL;
        }

        cardFrom->prev = prevNode;
        if (prevNode != NULL) {
            prevNode->next = cardFrom;
        } else {
            to->head = cardFrom;
        }

        to->tail = lastMovedCard;
        to->size += cardsMoved;

        result = true; // Successful move
    }

    return result;
}

// Function to delete an entire linked list
void deleteLinkedList(Linked_list *list) {
    while (list->size > 0) {
        removeNode(list); // Remove all nodes one by one
    }

    free(list); // Finally, free the list structure
}

// Function to find a specific node based on card value and suit
struct ListCard *findNodeFromCard(Linked_list *list, char value, char suit) {
    struct ListCard *card = list->head; // Start searching from the head
    while (card != NULL) {
        if (card->value == value && card->suit == suit) {
            return card; // Card found
        }
        card = card->next; // Move to next card
    }
    return NULL; // Card not found
}

// Function to print the linked list contents to the console
void LinkedListToString(Linked_list *list) {
    struct ListCard *card = list->head;
    while (card != NULL) {
        printf("%c%c\n", card->value, card->suit); // Print card value and suit
        card = card->next; // Move to next card
    }
}
