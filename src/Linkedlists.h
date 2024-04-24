// Defining only if not defined
#ifndef YUKON_LINKEDLIST_H
#define YUKON_LINKEDLIST_H



#include <stdio.h>

#include <stdlib.h>

#include <stdbool.h>



// Definition of struct for Card
struct ListCard{

    char value;
    char suit;

    bool faceDown;
    bool existsInGame;

    struct ListCard *next;
    struct ListCard *prev;
};


// Structure of Linked List

typedef struct{

    struct ListCard *tail;
    struct ListCard *head;

    int size;
    
} LinkedList;
















#endif