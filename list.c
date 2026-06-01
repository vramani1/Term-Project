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

void addLast(LIST *lp, void *item) {
    assert(lp != NULL);
    
    if (lp->tail == NULL || lp->tail->count == lp->tail->length) {
        int new_capacity = (lp->tail == NULL) ? INITIAL_CAPACITY : (lp->tail->length * 2);
        NODE *newNode = createNode(new_capacity);
        
        if (lp->tail == NULL) { 
            lp->head = newNode;
            lp->tail = newNode;
        } else {             
            newNode->prev = lp->tail;
            lp->tail->next = newNode;
            lp->tail = newNode;
        }
    }
    
    NODE *tail = lp->tail;
    int insertIndex = (tail->first + tail->count) % tail->length;
    tail->data[insertIndex] = item;
    
    tail->count++;
    lp->count++;
}

void addFirst(LIST *lp, void *item) {
    assert(lp != NULL);

    if (lp->head == NULL || lp->head->count == lp->head->length) {
        int new_capacity = (lp->head == NULL) ? INITIAL_CAPACITY : (lp->head->length * 2);
        NODE *newNode = createNode(new_capacity);
        
        if (lp->head == NULL) { 
            lp->head = newNode;
            lp->tail = newNode;
        } else {               
            newNode->next = lp->head;
            lp->head->prev = newNode;
            lp->head = newNode;
        }
    }
    
    NODE *head = lp->head;
    head->first = (head->first - 1 + head->length) % head->length; 
    head->data[head->first] = item;
    
    head->count++;
    lp->count++;
}

void *getItem(LIST *lp, int index) {
    assert(lp != NULL);
    assert(index >= 0 && index < lp->count);
    
    NODE *current = lp->head;
    
    // Traverse nodes, subtracting node counts from our target index
    while (index >= current->count) {
        index -= current->count;
        current = current->next;
    }
    
    // We found the correct node. Now grab the item using circular math.
    int targetIndex = (current->first + index) % current->length;
    return current->data[targetIndex];
}
