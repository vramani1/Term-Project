#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "list.h" 

#define INITIAL_CAPACITY 10

typedef struct node {
    void **data;        
    int length;         
    int count;         
    int first;          
    struct node *next;  
    struct node *prev;  
} NODE;

struct list {
    NODE *head;
    NODE *tail;
    int count;
};

NODE *createNode(int capacity) {
    NODE *newNode = malloc(sizeof(NODE));
    assert(newNode != NULL);
    
    newNode->data = malloc(sizeof(void *) * capacity);
    assert(newNode->data != NULL);
    
    newNode->length = capacity;
    newNode->count = 0;
    newNode->first = 0;
    newNode->next = NULL;
    newNode->prev = NULL;
    
    return newNode;
}

LIST *createList(void) {
    LIST *lp = malloc(sizeof(struct list));
    assert(lp != NULL);
    lp->head = NULL;
    lp->tail = NULL;
    lp->count = 0;
    return lp;
}

void destroyList(LIST *lp) {
    assert(lp != NULL);
    NODE *current = lp->head;
    while (current != NULL) {
        NODE *next = current->next;
        free(current->data); // Free the inner array
        free(current);       // Free the node
        current = next;
    }
    free(lp);
}

int numItems(LIST *lp) {
    assert(lp != NULL);
    return lp->count;
}
