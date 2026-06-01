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
