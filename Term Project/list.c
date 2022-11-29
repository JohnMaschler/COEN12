//Johnathon Maschler COEN 12 Term Project Prof. Amer - June 4, 2022

# include <stdio.h>
# include <stdlib.h>
# include <assert.h>
# include "list.h"
//The purpose of this lab is to create a linked list where each node is pointing to an array that is a queue and as the list grows,
//the length of the new array doubles. Stack and Queue functions were implemented for this list

//struct for the doubly linked, circular linked list
struct node{
    struct node *next;
    struct node *prev;
    void **array;
    int length;
    int count;
    int first;
};

//struct for the queue
struct list{
    struct node *head;
    int count;
    int total;//keep track of lp->total++ number of items to returnq
};

struct node *createNode(int length)//O(1) function to create a node each time a new one needs to be created
{
    struct node *np;
    np = malloc(sizeof(struct node));
    assert(np!=NULL);
    np->length = length;
    np->count=0;
    np->array = malloc(sizeof(void*)*(length));
    np->first=0;
    return np;
}

LIST *createList(void)//O(1) function to create and return a pointer to a new list
{
    LIST *lp;
    lp = malloc(sizeof(struct list));
    assert(lp!=NULL);

    lp->head = createNode(8);

    lp->count = 0;
    lp->total = 0;

    lp->head->next=lp->head;
    lp->head->prev=lp->head;

    return lp;
}

void destroyList(LIST *lp) //O(log n) deallocate memory associated with the list pointed to by lp
{
    assert(lp!=NULL);
    struct node *dummy = lp->head;

    do{
        free(dummy);
        dummy=dummy->next;
    }while(dummy!=lp->head);
    free(lp);
    return;
}

int numItems(LIST *lp)//O(1) return the number of items in the list pointed to by lp
{
    assert(lp!=NULL);
    return(lp->total);
}

void addFirst(LIST *lp, void *item)//O(1) add item as the first item in the list pointed to by lp
{
    assert(lp!=NULL&&item!=NULL);
    struct node *dummy = lp->head->next;

    if (lp->count == 0||dummy->count==dummy->length)
    {
        struct node *temp = malloc(sizeof(struct node));
        temp = createNode(((dummy->next->length)*2));
        temp->next=dummy;
        temp->prev = dummy->prev;
        dummy->prev->next=temp;
        dummy->prev=temp;
        lp->count++;
        temp->array[0] = item;
        temp->count++;
        lp->total++;
        return;
    }
    dummy->first = (dummy->first-1+dummy->length)%dummy->length;
    dummy->array[dummy->first] = item;
    dummy->count++;
    lp->total++;

    return;
}

void addLast(LIST *lp, void *item)//O(log n) add item as the last item in the list pointed to by lp
{
    assert(lp!=NULL && item!=NULL);
    struct node *dummy=lp->head->prev;

    if (lp->count == 0)
    {
        struct node *temp = malloc(sizeof(struct node));
        temp = createNode(((dummy->prev->length)*2));
        temp->prev=dummy->prev;
        temp->next=dummy;
        dummy->prev->next=temp;
        dummy->prev=temp;
        lp->count++;
        temp->array[0] = item;
        temp->count++;
        lp->total++;
        return;
    }
    while (dummy->count == dummy->length)
        dummy = dummy->prev;

    if (dummy == lp->head)
    {
        struct node *temp = malloc(sizeof(struct node));
        temp = createNode(((dummy->prev->length)*2));
        temp->prev=dummy->prev;
        temp->next=dummy;
        dummy->prev->next=temp;
        dummy->prev=temp;
        lp->count++;
        temp->array[0] = item;
        temp->count++;
        lp->total++;
        return;   
    }
    dummy->array[(dummy->first+dummy->count++)%dummy->length] = item;
    lp->total++;
    return;
}

void *removeFirst(LIST *lp)//O(1) remove and return the first item in the list pointed to by lp ; the list must not be empty
{
    assert(lp!=NULL);
    struct node *dummy = lp->head->next;
    void *item;

    item = dummy->array[dummy->first%dummy->length];
    dummy->first++;
    dummy->first = dummy->first%dummy->length;
    dummy->count--;
    lp->total--;

    if (dummy->count==0)
    {
        dummy->prev->next = dummy->next;
        dummy->next->prev = dummy->prev;
        free(dummy);
        lp->count--;
    }
    
    return item;
}

void *removeLast(LIST *lp)//O(1) remove and return the last item in the list pointed to by lp ; the list must not be empty
{
    assert(lp!=NULL);
    void *item;

    struct node *dummy=lp->head->prev;

    item = dummy->array[(dummy->first+dummy->count-1)%dummy->length];
    lp->total--;
    dummy->count--;

    if (dummy->count==0)
    {
        dummy->next->prev = dummy->prev;
        dummy->prev->next = dummy->next;
        free(dummy);
        lp->count--;
    }

    return item;
}

void *getFirst(LIST *lp)//O(1) return the first item in the list pointed to by lp
{
    assert(lp!=NULL);
    struct node *dummy = lp->head->next;
    void *item;
    item = dummy->array[dummy->first%dummy->length];
    return item;
}

void *getLast(LIST *lp)//O(1) return the last item in the list pointed to by lp
{
    assert(lp!=NULL);
    struct node *dummy = lp->head->prev;
    void *item;
    item = dummy->array[(dummy->first+dummy->count-1)%dummy->length];
    return item;
}

void *getItem(LIST *lp, int index)//O(logn) return the item at position index in the list pointed to by lp; the index must be within range
{
    assert(lp!=NULL);
    struct node *dummy = lp->head->next;

    while((dummy->count<=index) && (dummy!=lp->head))
    {
        index = index - dummy->count;
        dummy=dummy->next;
    }
    void *item = dummy->array[(dummy->first+index)%(dummy->length)];
    return item;
}

void setItem(LIST *lp, int index, void *item)//O(logn) set the given index to the item passed as an argument
{
    assert(lp!=NULL);
    struct node *dummy = lp->head->next;

    while((dummy->count<=index) && (dummy!=lp->head))
    {
        index = index - dummy->count;
        dummy=dummy->next;
    }
    dummy->array[(dummy->first+index)%(dummy->length)] = item;
    return;
}