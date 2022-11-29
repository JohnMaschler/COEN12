//Johnathon Maschler COEN12 Lab 5 Thurs 9:15 Prof. Amer

/*
In this lab we are creating a priority queue that will repeatedly remove
and print the smallest value from the queue so that all of the integers
will be in sorted order.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "pqueue.h"

struct pqueue{
    int count; /* number of entries in array*/
    int length; /* length of allocated array  */    
    void **data; /* allocated array of entries */    
    int (*compare)(); /* comparison function*/
};
int Parent(int);
int Left_child(int);
int Right_child(int);

int Parent(int x) {return (((x) - 1) / 2);}//implemented parent and child as functions to easily input and return values O(1)
int Left_child(int x) {return ((x) * 2 + 1);}
int Right_child(int x) {return ((x) * 2 + 2);}

PQ *createQueue(int (*compare)())//O(1)return a pointer to a new priority queue using compare as its comparison function
{
    PQ *pq;
    pq = malloc(sizeof(struct pqueue));
    assert (pq!=NULL);
    pq->length = 10;
    pq->count = 0;
    pq->compare=compare;
    assert(compare!=NULL);
	pq->data = malloc(sizeof(void*)*pq->length);
	return pq;
};

void destroyQueue(PQ *pq)//O(1) deallocate memory associated with the priority queue pointed to by pq
{
    assert(pq!=NULL);
    free(pq->data);
    free(pq);
    return;
}
int numEntries(PQ *pq)//O(1) return the number of entries in the priority queue pointed to by pq
{
    assert(pq!=NULL);
    return pq->count;
}
void addEntry(PQ *pq, void *entry)//O(log n) add entry to the priority queue pointed to by pq
{
    assert(pq!=NULL&&entry!=NULL);

    if (pq->length==pq->count)
    {
        pq->length*=2;
        pq->data = realloc(pq->data, sizeof(void *)*pq->length);
        assert(pq->data!=NULL);
    }
    pq->data[pq->count]=entry;
    int child=pq->count;
    pq->count++;

    while((pq->compare(pq->data[child], pq->data[Parent(child)])<0) && child>0)
    {
        void *temp = pq->data[Parent(child)];
        pq->data[Parent(child)] = pq->data[child]; 
        pq->data[child] = temp;
        child = Parent(child);
    }
    return;
}
void *removeEntry(PQ *pq)//O(log n) remove and return the smallest entry from the priority queue pointed to by pq
{
    assert(pq != NULL);
    void *dummy;
    int smallest;
    void* temp;
    dummy = pq->data[0];
    int location=0;
    pq->data[0] = pq->data[pq->count-1];
    pq->count--;
    while(Left_child(location) < pq->count)
    {
        smallest = Left_child(location);
        if (Right_child(location) < pq->count)
        {
            if (pq->compare(pq->data[Left_child(location)], pq->data[Right_child(location)])>0)
                smallest = Right_child(location);
        }
        if (pq->compare(pq->data[smallest], pq->data[location]) < 0)
        {
            temp = pq->data[smallest]; 
            pq->data[smallest] = pq->data[location];
            pq->data[location] = temp; 
            location=smallest;
        }
        else
            break;
    }
    return dummy;
}