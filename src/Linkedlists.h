#ifndef YUKON_LINKEDLIST_H
#define YUKON_LINKEDLIST_H

#include <stdio.h>

#include <stdlib.h>

#include <stdbool.h>

struct ListCard{

    char value;
    char suit;

    bool faceDown;
    bool existsInGame;

    struct ListCard *next;
    struct ListCard *prev;
    bool exists;
};


typedef struct{
    int size;
    struct ListCard *tail;
    struct ListCard *head;


} Linked_list;

Linked_list *createLinkedList();

void appendCard(Linked_list *list, struct ListCard card);

void prependCard(Linked_list *list, struct ListCard card);

void insertNode(Linked_list *list, struct ListCard *nodeToInsert, struct ListCard *previousNode, bool insertBefore);

void removeNode(Linked_list *list);

void deleteLinkedList(Linked_list *list);

void LinkedListToString(Linked_list *list);

struct ListCard *findNodeFromCard(Linked_list *list, char value, char suit);

bool moveCardBetweenLists(Linked_list *from, struct ListCard *cardFrom, Linked_list *to);

#endif //YUKON_LINKEDLIST_H