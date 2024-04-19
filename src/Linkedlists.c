#include "Linkedlists.h"


Linked_list *createLinkedList() {
    Linked_list *newLinkedList;
    newLinkedList = (Linked_list *) malloc(sizeof(Linked_list));

    newLinkedList->size = 0;
    newLinkedList->head = NULL;
    newLinkedList->tail = NULL;

    return newLinkedList;
}

/**
 * Add a new node to the end of and existing list based on the card pointer.
 * @author s215797 Mikael Fangel
 * @param list list to add new node to
 * @param card card to add as node to the list
 */
void appendCard(Linked_list *list, struct ListCard card) {
    struct ListCard *newCard = (struct ListCard *) malloc(sizeof(struct ListCard));

    newCard->suit = card.suit;
    newCard->value = card.value;
    newCard->existsInGame = card.existsInGame;
    newCard->faceDown = card.faceDown;

    if (list->size == 0) {
        newCard->next = NULL;
        newCard->prev = NULL;
        list->head = newCard;
        list->tail = newCard;
    } else {
        newCard->prev = list->tail;
        list->tail->next = newCard;
        newCard->next = NULL;
        list->tail = newCard;
    }

    list->size++;
}

/**
 * Add a new node to the begging of and existing list based on the card pointer.
 * @author s215797 Mikael Fangel
 * @param list list to add new node to
 * @param card card to add as node to the list
 */
void prependCard(Linked_list *list, struct ListCard card) {
    struct ListCard *newCard = (struct ListCard *) malloc(sizeof(struct ListCard));

    newCard->suit = card.suit;
    newCard->value = card.value;
    newCard->existsInGame = card.existsInGame;
    newCard->faceDown = card.faceDown;

    if (list->size == 0) {
        newCard->next = NULL;
        newCard->prev = NULL;
        list->head = newCard;
        list->tail = newCard;
    } else {
        newCard->next = list->head;
        list->head->prev = newCard;
        newCard->prev = NULL;
        list->head = newCard;
    }

    list->size++;
}

/**
 * Inserts a node in an existing linked list either before of after another specified node in the list.
 * @author s215797 Mikael Fangel
 * @param list list to perform insert on
 * @param nodeToInsert node to added to linked list
 * @param previousNode node before the node to insert
 * @param insertBefore should the node be insert before or after
 */
void insertNode(Linked_list *list, struct ListCard *nodeToInsert, struct ListCard *previousNode, bool insertBefore) {
    struct ListCard *nodeCopy = (struct ListCard *) malloc(sizeof(struct ListCard));

    *nodeCopy = *nodeToInsert;
    if (insertBefore && list->size > 0) {
        if (previousNode->prev != NULL) {
            nodeCopy->next = previousNode;
            nodeCopy->prev = previousNode->prev;

            previousNode->prev->next = nodeCopy;
            previousNode->prev = nodeCopy;
        } else {
            prependCard(list, *nodeToInsert);
            free(nodeCopy);
            list->size--;
        }
    } else {
        if (list->size > 0 && previousNode->next != NULL) {
            nodeCopy->next = previousNode->next;
            nodeCopy->prev = previousNode;

            previousNode->next->prev = nodeCopy;
            previousNode->next = nodeCopy;
        } else {
            appendCard(list, *nodeToInsert);
            free(nodeCopy);
            list->size--;
        }
    }

    list->size++;
}

/**
 * Removes a node of the end of the list.
 * @author s215797 Mikael Fangel
 * @param list list to remove node from
 */
void removeNode(Linked_list *list) {
    if (list->size == 0) {
        return;
    } else {
        struct ListCard *card = list->tail;
        if (card->prev != NULL) {
            struct ListCard *prevNode = card->prev;
            prevNode->next = NULL;
            list->tail = prevNode;
        } else {
            list->tail = NULL;
            list->head = NULL;
        }

        free(card);
        list->size--;
    }
}

/**
 * Delete an entire linked list and all of its nodes using the remove node function
 * @author s215797 Mikael Fangel
 * @param list list to the delete
 */
void deleteLinkedList(Linked_list *list) {
    while (list->size > 0) {
        removeNode(list);
    }

    free(list);
}

/**
 * Finds a node in a linked list based on the searchCard value
 * @authors s215805 Mads Sørensen (20%), s215797 Mikael Fangel (80%)
 * @param list list to search
 * @param searchCard searchCard to find
 * @return the node in the linked list if found and NULL if node is not in the list
 */
struct ListCard *findNodeFromCard(Linked_list *list, char value, char suit) {
    struct ListCard *card = list->head;
    while (card != NULL) {
        if (card->value == value && card->suit == suit) {
            return card;
        }
        card = card->next;
    }
    return NULL;
}

/**
 * Moves a card from anywhere in a linked list to the end of another
 * @author s215797 Mikael Fangel
 * @param from list to move from
 * @param cardFrom card to move
 * @param to list to move to
 * @return true if successful and fall if unsuccessful
 */
bool moveCardFromOneLinkedListToAnother(Linked_list *from, struct ListCard *cardFrom, Linked_list *to) {
    bool result = false;
    struct ListCard *prevNode = to->tail;

    // Checks how many nodes that are going to be moved so that we can calculate the new list size
    int cardsMoved = 1;
    struct ListCard *lastMovedCard = cardFrom;
    while (lastMovedCard->next != NULL) {
        cardsMoved++;
        lastMovedCard = lastMovedCard->next;
    }

    // Ends function if not found and to allow error handling
    if (cardFrom != NULL) {
        // Detach node from its list
        if (cardFrom->prev != NULL)
            cardFrom->prev->next = NULL;

        from->tail = cardFrom->prev;
        from->size = from->size - cardsMoved;

        if (from->size == 0) {
            from->head = NULL;
        }

        // Linking
        cardFrom->prev = prevNode;
        if (prevNode != NULL) {
            prevNode->next = cardFrom;
        } else {
            to->head = cardFrom;
        }

        to->tail = lastMovedCard;
        to->size = to->size + cardsMoved;

        result = true;
    }

    return result;
}

/**
 * Prints the cards each on one new line
 * @authors s215805 Mads Sørensen (90%), s215797 Mikael Fangel (10%)
 * @param list
 */
void LinkedListToString(Linked_list *list) {
    struct ListCard *card = list->head;
    while (card != NULL) {
        printf("%c%c\n", card->value, card->suit);
        card = card->next;
    }
}
