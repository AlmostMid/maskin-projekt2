#include "Linkedlists.h"


Linked_list *createLinkedList() {
    Linked_list *newLinkedList;
    newLinkedList = (Linked_list *) malloc(sizeof(Linked_list));

    newLinkedList->size = 0;
    newLinkedList->head = NULL;
    newLinkedList->tail = NULL;

    return newLinkedList;
}

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

void deleteLinkedList(Linked_list *list) {
    while (list->size > 0) {
        removeNode(list);
    }

    free(list);
}

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

void LinkedListToString(Linked_list *list) {
    struct ListCard *card = list->head;
    while (card != NULL) {
        printf("%c%c\n", card->value, card->suit);
        card = card->next;
    }
}
