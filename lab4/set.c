//Johnathon Maschler COEN 12 Lab 4 Prof. Amer Th 9:15

/*In this lab we will be using a hash table with a linked list
in order to remove collisions and improve the efficiency of our function*/

#include "set.h"
#include "list.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>


struct set//create struct set that will hold the elements of the set
{
    int count;
    int length;
    LIST** ls;
    int (*compare)();
    unsigned (*hash)();
};

SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)())// O(m) creating set by allocating memory for it and defining each member of the struct
{
    SET *sp;
    sp = malloc(sizeof(SET));
    assert(sp!=NULL);
    sp->count=0;
    sp->compare=compare;
    sp->length=maxElts;
    sp->ls = malloc(sizeof(LIST*)*sp->length);
    int i;
    for (i=0; i<sp->length;i++)
    {
        sp->ls[i]=createList(sp->compare);
    }
    assert(sp->ls!=NULL);
    sp->hash=hash;
    return sp;
}

void destroySet(SET *sp)//O(m) destroy the entire set by freeing all of the space allocated to the set and the set's elements
{
    assert(sp!=NULL);
    int i;
    for (i=0;i<sp->length;i++)
        destroyList(sp->ls[i]);
    free(sp->ls);
    free(sp);
}

int numElements(SET *sp)// 0(1) return the number of elements in the set
{
    assert(sp!=NULL);
    return sp->count;
}

void addElement(SET *sp, void *elt)//O(n) worst case. Expected O(1). Use findItem function to locate an item, if it's not located then we add it, else we just return the function
{
    assert(sp->count<sp->length);
    assert(sp!=NULL&&elt!=NULL);

    void* temp;
    int val;
    val = sp->hash(elt)%sp->length;
    temp=findItem(sp->ls[val], elt);
    if (temp==NULL)
    {   
        addFirst(sp->ls[val], elt);
        sp->count++;
    }
    return;
}

void removeElement(SET *sp, void *elt)//O(n) worst case. Expected O(1) remove an element from the set by searching for the element and freeing the space
{
    assert(sp!=NULL&&elt!=NULL);
    void* temp;
    int val;
    val = sp->hash(elt)%sp->length;
    temp = findItem(sp->ls[val], elt);
    if (temp!=NULL)
    {
        removeItem(sp->ls[val], elt);
        sp->count--;
    }
    return;
}

void *findElement(SET *sp, void *elt)//O(n) worst case. Expected O(1). find an element that is in the set by comparing two strings and returning the string or NULL
{
    assert(sp!=NULL&&elt!=NULL);
    int val;
    void* find;
    val = sp->hash(elt)%sp->length;
    find = findItem(sp->ls[val], elt);
    return find;
}

void *getElements(SET *sp)//O(m) creating a copy of the elements, allocating space for the copy and returning the copy of all the elements
{
    assert(sp!=NULL);
    void **cpyElts;
    cpyElts = malloc(sizeof(void*)*sp->count);
    assert(cpyElts!=NULL);
    int i,j=0;
    void **temp;
    for (i=0;i<sp->length;i++)
    {
        temp=getItems(sp->ls[i]);
        memcpy(cpyElts+j, temp, sizeof(void*)*numItems(sp->ls[i]));
        j+=numItems(sp->ls[i]);
        free (temp);
    }
    return cpyElts;
}