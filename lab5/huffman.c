//Johnathon Maschler COEN 12 Lab 9:15 Thurs. Prof Amer


/*
Huffman Coding project with driver program that will take
in an input file as the argument and will compress the input file to an output file with .z extension
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "pqueue.h"
#include "pack.h"

#define NODE struct node

int nodeCmp(NODE*, NODE*);
int depthNode(NODE*);


NODE *mknode(int data, NODE *left_node, NODE *right_node)//O(1)private function to make a new node
{
    NODE *new_node = malloc(sizeof(NODE));
    new_node->count=data;
    new_node->parent=NULL;
    if (left_node!=NULL)
        left_node->parent=new_node;
    if (right_node!=NULL)
        right_node->parent=new_node;
    return new_node;
}

int nodeCmp(NODE *t1, NODE *t2)//O(1)a comparison function for createQueue(cmp)
{
    assert(t1!=NULL&&t2!=NULL);
    return (t1->count < t2->count) ? -1 : (t1->count > t2->count);
}

int depthNode(NODE *node)//O(n) recursive function used to calculate the number of bits
{
    return node->parent==NULL ? 0 : 1 + depthNode(node->parent);
}

int main(int argc, char *argv[])
{
    if (argc!=3)
    {
        printf("error, not enough args\n");
        return 0;
    }

    int counts[257];//integer array to store the occurences for the characters
    NODE *nodes[257];//array of nodes

    int i;
    for (i=0; i<257; i++)
    {
        counts[i]=0;
        nodes[i]=NULL;
    }
    
    FILE *fp;
    fp=fopen(argv[1], "r");
    if (fp==NULL)
    {
        printf("error\n");
        return 0;
    }
    int ch;
    while((ch=getc(fp))!=EOF)//counting the number of occurences of each character in the file and keeping track using the counts array
        counts[ch]++;
    fclose(fp);

    PQ *pq;
    pq=createQueue(nodeCmp);
    int j;
    for(j=0;j<256;j++)//creating a priority queue to build the Huffman tree from the bottom up (leaves to root)
    {
        if (counts[j]>0)
        {
            nodes[j] = mknode(counts[j], NULL, NULL);
            addEntry(pq, nodes[j]);
        }
    }
    nodes[256] = mknode(counts[256], NULL, NULL);//An extra node created for EOF with zero count
    addEntry(pq, nodes[256]);



    while(numEntries(pq)>1)//Building the tree, by taking first two nodes out and creating a new one with the count of the sum of the two, then putting the new one back until there's only one left.
    {
        NODE *left_node = removeEntry(pq);
        NODE *right_node = removeEntry(pq);
        NODE *parent_node = mknode(left_node->count+right_node->count, left_node, right_node);
        addEntry(pq, parent_node);
    }


    for (i=0; i<=256; i++)//printing the occurences and length of bits for each character. Makes use of depth function to calculate number of bits
    {
        if (nodes[i]!=NULL)
        {
            printf(isprint(i)? "'%c'" : "%03o", i);
            printf(": %d x %d = %d\n", counts[i], depthNode(nodes[i]), counts[i]*depthNode(nodes[i]));
        }
    }
    pack(argv[1], argv[2], nodes);//calling pack to generate the compressed file
}
