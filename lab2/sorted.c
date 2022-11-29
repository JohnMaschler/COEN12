#include "set.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

static int search(SET *sp, char *elt, bool *found);//search function that will be used throughout the code

struct set//create struct set that will hold the elements, count, and length
{
    int count;
    int length;
    char **elts;
};

SET *createSet(int maxElts)// O(1) creating set by allocating memory for it and defining each member of the struct
{
    asserts;
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
    assert(sp!=NULL&elt!=NULL);

    int findit;
    bool found;
    int i;
    findit = search(sp,elt,&found);
    if (found==false)
    {   
        for (i=sp->count;i>findit;i--)
            sp->elts[i]=sp->elts[i-1];

        sp->elts[findit] = strdup(elt);
        assert(sp->elts[findit]!=NULL);
        sp->count++;
    }
    return;
}

void removeElement(SET *sp, char *elt)//O(n) remove an element from the set by searching for the element and freeing the space
{
    int i;
    int x;
    bool found;
    assert(sp!=NULL&elt!=NULL);
    x=search(sp, elt, &found);
    if (found==true)
    {
        free(sp->elts[x]);
        for (i=x;i<sp->count-1;i++)
        {
            sp->elts[i]=sp->elts[i+1];//shift the array over
        }
        sp->count--;
    }
}

char *findElement(SET *sp, char *elt)//O(logn)find an element that is in the set by comparing two strings and returning the string or NULL
{
    assert(sp!=NULL&elt!=NULL);
    bool found;
    int findelt = search(sp, elt, &found);

    if (found==true)
        return sp->elts[findelt];

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

static int search(SET *sp, char elt[], bool *found)//O(logn) binary search helper function that returns the location of a string with a boolean variable passed by reference
{
    assert(sp!=NULL&elt!=NULL);
    int i;
    int high=sp->count-1, low=0, mid;

    while (low<=high)
    {
        mid = (high+low)/2;
        int dif=strcmp(elt, sp->elts[mid]);
        if (dif==0)
        {
            *found=true;
            return mid;
        }
        else if (dif>0)
            low=mid+1;

        else if (dif<0)
            high=mid-1;
    }
    *found=false;
    return low;
}