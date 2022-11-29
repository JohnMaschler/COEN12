//Johnathon Maschler Lab 4 COEN 12 Professor Amer

/*The purpose of this lab is to implement a stack 
using a doubly-ended queue abstract data type so that items can be easily
removed from the beginning and end of the list (this includes the use of a sentinel node as well).
This will then be used to implement hashing with chaining*/

# include <time.h>	
# include <stdio.h>
# include <stdlib.h>
# include <assert.h>
# include <curses.h>
# include <unistd.h>
# include <stdbool.h>
# include "list.h"

//a struct that contains the elements of the list
struct list{
    int count;
    struct node *head;
    int (*compare)();
};

//struct that contains the elements of each node
struct node{
    void *data;
    struct node *next;
    struct node *prev;
};
typedef struct list LIST;

LIST *createList(int (*compare)())//O(1)creating a new list to be used—using the given function compare as the comparison function
{
    LIST *lp;
    lp = malloc(sizeof(struct list));
    assert (lp!=NULL);
    lp->count=0;
    lp->compare=compare;
    lp->head = malloc(sizeof(struct node));
    assert(lp->head!=NULL);
    lp->head->next = lp->head;
    lp->head->prev = lp->head;

    return lp;
}
void destroyList(LIST *lp)//O(n) Deallocating the memory associated with the list pointed to by lp
{
    assert(lp!=NULL);
    struct node *dummy = lp->head;
    struct node *next;
    next=malloc(sizeof(struct node));

    do{
        next = dummy->next;
        free(dummy);
        dummy = next;
    } while (dummy!=lp->head);
    free (lp);
    return;
}
int numItems(LIST *lp)//O(1)Return the number of items in the list
{
    assert(lp!=NULL);
    return lp->count;
}
void addFirst(LIST *lp, void *item)//O(1) This function adds an element passed as an argument as the first item in the list pointed to by lp
{
    assert(lp!=NULL&&item!=NULL);
    struct node *dummy = lp->head;
    struct node *temp;
    temp=malloc(sizeof(struct node));
    temp->data = item;
    lp->count++;
    temp->next=dummy->next;
    temp->prev=dummy;
    dummy->next->prev=temp;
    dummy->next=temp;
    return;
}
void addLast(LIST *lp, void *item)//O(1) Adds an element passed as an argument as the last item in the list pointed to by lp
{
    assert(lp!=NULL&&item!=NULL);
    struct node *dummy = lp->head;
    struct node *temp;
    temp=malloc(sizeof(struct node));
    assert(temp!=NULL);
    temp->data=item;
    temp->next=dummy;
    temp->prev=dummy->prev;
    dummy->prev->next=temp;
    dummy->prev=temp;
    lp->count++;
    return;
}
void *removeFirst(LIST *lp)//O(1) Removing and returning the first item in the list pointed to by lp as long as the list is not empty
{
    assert(lp!=NULL);
    struct node *dummy = lp->head;
    struct node *temp = dummy->next;
    void *var = temp->data;
    temp->next->prev=dummy;
    dummy->next=temp->next;
    lp->count--;
    return var;
}
void *removeLast(LIST *lp)//O(1) Removing and returning the first item in the list pointed to by lp as long as the list is not empty
{
    assert(lp!=NULL);
    struct node *dummy = lp->head;
    struct node *temp = dummy->prev;
    void *var = temp->data;
    temp->prev->next=dummy;
    dummy->prev=temp->prev;
    lp->count--;
    return var;
}
void *getFirst(LIST *lp)//O(1) Returns the first item in non-empty list (does not remove it)
{
    assert(lp!=NULL);
    struct node *dummy = lp->head;
    struct node *temp = dummy->next;
    void *var = temp->data;
    return var;
}
void *getLast(LIST *lp)//O(1) Returns the last item in non-empty list (does not remove it)
{
    assert(lp!=NULL);
    struct node *dummy = lp->head;
    struct node *temp=dummy->prev;
    void *var = temp->data;
    return var;
}


void removeItem(LIST *lp, void *item)//O(n) Uses the comparison function to check if a given item is in the list, if it is then it is removed
{
    assert(lp!=NULL&&lp->compare!=NULL);
    struct node *dummy = lp->head->next;
    while (dummy!=lp->head)
    {
        if (lp->compare(dummy->data, item)==0)
        {
            lp->count--;
            dummy->prev->next=dummy->next;
            dummy->next->prev=dummy->prev;
            free(dummy);
        }
        dummy=dummy->next;
    } 
    return;
}
void *findItem(LIST *lp, void *item)//O(n) Checks to see if a given item is in the list, if it is then the item is returned
{
    assert(lp!=NULL&&lp->compare!=NULL);
    struct node *dummy = lp->head->next;
    struct node *next;
    while(dummy!=lp->head)
    {
        next=dummy->next;
        if (lp->compare(dummy->data, item)==0)
            return dummy->data;
        dummy=next;
    }
    return NULL;
}
void *getItems(LIST *lp)//O(n) Allocating memory and returning an array of items in the list pointed to by lp
{
    assert(lp!=NULL);
    struct node *dummy=lp->head->next;
    void** darray;
    darray=malloc(sizeof(void*)*lp->count);
    int i=0;
    while(dummy!=lp->head)
    {
        darray[i]=dummy->data;
        dummy=dummy->next;
        i++;
    }
    return darray;
}
