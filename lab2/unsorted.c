#include "set.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>


static int search(SET *sp, char *elt);//search function that will be used throughout the code

struct set//create struct set that will hold the elements, count, and length
{
    int count;
    int length;
    char **elts;
};

SET *createSet(int maxElts)// O(1) creating set by allocating memory for it and defining each member of the struct
{
    SET *sp;
    sp = malloc(sizeof(SET));
    sp->count=0;
    sp->length=maxElts;
    sp->elts = malloc(sizeof(char*)*maxElts);
    assert(sp->elts!=NULL);
    
    return sp;
}

void destroySet(SET *sp)//O(n) destroy the entire set by freeing all of the space allocated to the set and the set's elements
{
    int i;
    assert(sp!=NULL);
    for (i=0;i<sp->count;i++)
        free(sp->elts[i]);
    free(sp->elts);
    free(sp);
}

int numElements(SET *sp)// 0(1) return the number of elements in the set
{
    assert(sp!=NULL);
    return sp->count;
}

void addElement(SET *sp, char *elt)//0(n) confirm that there are no duplicates and then add the element to the set
{
    assert(sp->count<sp->length);
    int checkDup;
    checkDup = search(sp,elt);

    if (checkDup==-1)
    {    
        sp->elts[sp->count++] = strdup(elt);
        assert(sp->elts[sp->count-1]!=NULL);
    }
    return;
}

void removeElement(SET *sp, char *elt)//O(n) remove an element from the set by searching for the element and freeing the space
{
    int i, j;
    int x;
    assert(sp!=NULL);
    x=search(sp, elt);
    if (x!=-1)
    {
        free(sp->elts[x]);
        sp->elts[x]=sp->elts[sp->count-1];//take the last element. no need to shift entire array because it is unsorted
        sp->count--;
    }
}

char *findElement(SET *sp, char *elt)//O(n)find an element that is in the set by comparing two strings and returning the string or NULL
{
    assert(sp!=NULL);
    int i;
    for (i=0;i<sp->count;i++)
        if (strcmp(sp->elts[i], elt)==0)
            return sp->elts[i];
    return NULL;
}

char **getElements(SET *sp)//O(1)creating a copy of the elements, allocating space for the copy and returning the copy of all the elements
{
    assert(sp!=NULL);
    char **cpyElts;
    cpyElts = malloc(sizeof(char*)*sp->count);
    assert(cpyElts!=NULL);
    return memcpy(cpyElts, sp->elts, sizeof(char*)*sp->count);
}

static int search(SET *sp, char elt[])//O(n) helper function to search the set (linear search) and find an element or return that the element was not found
{
    assert(sp!=NULL);
    int i;
    for (i=0; i<sp->count;i++)
        if (strcmp(sp->elts[i], elt)==0)
            return i;
    return -1;
}