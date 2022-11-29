//Johnathon Maschler COEN 12 Lab 3 Prof. Amer Th 9:15

/*In this lab we will be using a hash function to hopefully
achieve much faster run times when iterating through
files of generic types and creating SETS*/

#include "set.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>


static int search(SET *sp, void *elt, bool *found);//search function that will be used throughout the code

struct set//create struct set that will hold the elements of the set
{
    int count;
    int length;
    void **data;
    char *flag;
    int (*compare)();
    unsigned (*hash)();
};

SET *createSet(int maxElts, int (*compare)(), unsigned (*hash)())// O(m) creating set by allocating memory for it and defining each member of the struct
{
    int i;
    SET *sp;
    sp = malloc(sizeof(SET));
    assert(sp!=NULL);
    sp->count=0;
    sp->length=maxElts;
    sp->data = malloc(sizeof(void*)*maxElts);
    sp->flag = malloc(sizeof(char)*maxElts);
    for (i=0;i<sp->length;i++)
        sp->flag[i] = 'E';
    assert(sp->data!=NULL);
    assert(sp->flag!=NULL);
    sp->compare=compare;
    sp->hash=hash;
    return sp;
}

void destroySet(SET *sp)//O(1) destroy the entire set by freeing all of the space allocated to the set and the set's elements
{
    int i;
    assert(sp!=NULL);
    free(sp->flag);
    free(sp->data);
    free(sp);
}

int numElements(SET *sp)// 0(1) return the number of elements in the set
{
    assert(sp!=NULL);
    return sp->count;
}

void addElement(SET *sp, void *elt)//O(m) worst case. Expected O(1). confirm that there are no duplicates and then add the element to the set
{
    assert(sp->count<sp->length);
    assert(sp!=NULL&&elt!=NULL);

    int findit;
    bool found=false;
    int i;
    findit = search(sp,elt,&found);
    if (found==false)
    {   
        sp->data[findit] = elt;
        sp->flag[findit]='F';
        assert(sp->data[findit]!=NULL);
        sp->count++;
    }
    return;
}

void removeElement(SET *sp, void *elt)//O(m) worst case. Expected O(1) remove an element from the set by searching for the element and freeing the space
{
    int i;
    int x;
    bool found=false;
    assert(sp!=NULL&&elt!=NULL);
    x=search(sp, elt, &found);
    if (found==true)
    {
        sp->flag[x]='D';
        sp->count--;
    }
}

void *findElement(SET *sp, void *elt)//O(m) worst case. Expected O(1). find an element that is in the set by comparing two strings and returning the string or NULL
{
    assert(sp!=NULL&&elt!=NULL);
    bool found=false;
    int findelt = search(sp, elt, &found);

    if (found==true)
        return sp->data[findelt];

    return NULL;
}

void *getElements(SET *sp)//O(m) creating a copy of the elements, allocating space for the copy and returning the copy of all the elements
{
    assert(sp!=NULL);
    void **cpyElts;
    cpyElts = malloc(sizeof(void*)*sp->count);
    assert(cpyElts!=NULL);
    int i, j=0;
    for (i=0; i<sp->length; i++)
        if (sp->flag[i]=='F')
        {
            cpyElts[j]=sp->data[i];
            j++;
        }
    return cpyElts;
}

static int search(SET *sp, void *elt, bool *found)//O(m) at worst if element does not exist in SET, expected O(1). Uses hashvalue to implement a hash table
{
    assert(sp!=NULL&&elt!=NULL);
    unsigned int hashVal = (*sp->hash)(elt);
    int hashPlace = hashVal%sp->length;

    int i, flag=-1;
    for (i=0; i<sp->length; i++)
    {
        hashPlace = (hashVal+i)%sp->length;
        if (sp->flag[hashPlace]=='F')
        {
            if ((*sp->compare)(sp->data[hashPlace], elt)==0)
            {
                *found=true;
                return hashPlace;
            }
        }
        else if (sp->flag[hashPlace]=='D')
        {
            if (flag==-1)
                flag=hashPlace;
        }
        else if (sp->flag[hashPlace]=='E')
        {
            
            if (flag>=0)
                return flag;
            return hashPlace;
        }
    }
    return hashPlace;
}