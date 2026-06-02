//Varun Ramani, 6/1/2025, Term Project, this project stores a circular queue through a linked list that storees nodes with arrays that double in size with each creation. 
//This allows for O(logn) search times

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

//destroys the whole list by first freeing the data then the node. O(logn) runtime
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

//Returns number of total data items in lp. O(1) runtime
int numItems(LIST *lp) {
    assert(lp != NULL);
    return lp->count;
}

//Adds an element, item, to the end of the data in lp, creating a new node at end of queue if there is no space left in the current one. O(1) runtime.
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

//Adds an element to the front of the circular queue in lp, creating a new node at front if full. O(1) runtime.
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

//Removes the first element in the queue, frees node if no more elements in node. O(1) runtime.
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

//Removes the last element in lps tail node, removing node if no more data in node. O(1) runtime.
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

//Returns the data inside index data slot in lp as a void pointer. O(logn) runtime since it skips through the doubling size lists.
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
