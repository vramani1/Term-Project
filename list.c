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

//creates and returns a new clean node with capacity spots of void pointers. O(1) runtime
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

//Creates a clean list struct. O(n) runtime 
LIST *createList(void) {
    LIST *lp = malloc(sizeof(struct list));
    assert(lp != NULL);
    lp->head = NULL;
    lp->tail = NULL;
    lp->count = 0;
    return lp;
}

//destroys the whole list by first freeing the 
void destroyList(LIST *lp) {
    assert(lp != NULL);
    NODE *current = lp->head;
    while (current != NULL) {
        NODE *next = current->next;
        free(current->data); 
        free(current);  
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

void *removeFirst(LIST *lp) {
    assert(lp != NULL && lp->count > 0);
    
    NODE *head = lp->head;
    void *item = head->data[head->first]; 
    
    // Step the first index forward circularly
    head->first = (head->first + 1) % head->length;
    head->count--;
    lp->count--;
    
    if (head->count == 0) {
        lp->head = head->next;
        if (lp->head != NULL) {
            lp->head->prev = NULL;
        } else {
            lp->tail = NULL;
        }
        free(head->data);
        free(head);
    }
    
    return item;
}

void *removeLast(LIST *lp) {
    assert(lp != NULL && lp->count > 0);
    
    NODE *tail = lp->tail;
    int lastIndex = (tail->first + tail->count - 1) % tail->length;
    void *item = tail->data[lastIndex]; 
    
    tail->count--;
    lp->count--;
    
    if (tail->count == 0) {
        lp->tail = tail->prev;
        if (lp->tail != NULL) {
            lp->tail->next = NULL;
        } else {
            lp->head = NULL; 
        }
        free(tail->data);
        free(tail);
    }
    
    return item;
}

void *getItem(LIST *lp, int index) {
    assert(lp != NULL);
    assert(index >= 0 && index < lp->count);
    
    NODE *current = lp->head;
    
    while (index >= current->count) {
        index -= current->count;
        current = current->next;
    }
    
    int targetIndex = (current->first + index) % current->length;
    return current->data[targetIndex];
}
