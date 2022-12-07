#include "maxHeapUtil.h"
#include <stdlib.h>
#include <stdio.h>


/*
  allocates memory for a new node.
  takes as input the character to store in the node and the frequency with which the character appears in the file.
  returns the newly created node.
*/
struct maxHeapNode *newNode(char character, unsigned int frequency){

    //allocate memory for a new node.
    struct maxHeapNode *node = (struct maxHeapNode *)malloc(sizeof(struct maxHeapNode));

    //set both children to NULL as the node at the moment is a leaf in the tree.
    node->leftChild = node->rightChild = NULL;

    //set the character and the frequency.
    node->character = character;
    node->frequency = frequency;

    //return the node.
    return node;
}


/*
  allocates memory for maxHeap of a given capacity.
  takes as input the capacity which is the max amount of items acceptable. 
  returns the newly created maxHeap.
*/
struct maxHeap *createMaxHeap(unsigned int capacity){

    //allocate memory for a new maxHeap.
    struct maxHeap *newMaxHeap = (struct maxHeap *)malloc(sizeof(struct maxHeap));
    
    //per definition a new maxHeap is always empty, which means of size 0.
    newMaxHeap->heapSize = 0;

    //set the heap capacity.
    newMaxHeap->capacity = capacity;

    //allocate memory for the array of nodes with size equal to the [max amount of items(capacity) *the size of a node].
    newMaxHeap->arrayOfNodes = (struct maxHeapNode **)malloc(newMaxHeap->capacity * sizeof(struct maxHeapNode *));
    return newMaxHeap;
}


/*
  swaps two nodes.
  takes as input the pointer to two nodes and swaps them.
  used in the maxHeapify to orded.
*/
void swapNodes(struct maxHeapNode **lowerNode, struct maxHeapNode **higherNode){
    struct maxHeapNode *temp = *higherNode;
    *higherNode = *lowerNode;
    *lowerNode = temp;
}


/*
 standard maxHeapify function, used to order items in the heap in order to have the min value as the first item.
  takes as input the maxHeap and the index of the item so heapify on.
*/
void maxHeapify(struct maxHeap *myMaxHeap, unsigned int index){
    //take index of smallest value and both the children
    int smallest = index;
    int leftChild  = 2 * index + 1;
    int rightChild = 2 * index + 2;
    
    /*
      check if the left child's value is lower than the current smallest value, if so the left child is the smallest.
      also checks that the children exist and are in the array.
    */
    if(leftChild < myMaxHeap->heapSize && 
       myMaxHeap->arrayOfNodes[leftChild]->frequency < myMaxHeap->arrayOfNodes[smallest]->frequency){
        smallest = leftChild;
       }
    
    //same as before but for the right child, this is in a separate if in orded to check if the rightChild is smaller than the leftChild too.
    if(rightChild < myMaxHeap->heapSize &&
       myMaxHeap->arrayOfNodes[rightChild]->frequency < myMaxHeap->arrayOfNodes[smallest]->frequency){
        smallest = rightChild;
       }
    
    /*
      if the original smallest has been updated 
      then we have to swap the original smallest and the actual smallest value
      then heapify again.
    */
    if(smallest != index){
        swapNodes(&myMaxHeap->arrayOfNodes[smallest],&myMaxHeap->arrayOfNodes[index]);
        maxHeapify(myMaxHeap,smallest);
    }

}


/*
  checks if the maxHeap is of size 1.
  takes as input the maxHeap.
  returns if it's true(1) or false(0).
*/
int isSizeOne(struct maxHeap *myMaxHeap){
    return (myMaxHeap->heapSize == 1);
}


/*
  extracts the min value from the heap and heapify.
  takes as input the maxHeap.
  returns the node with the lowest value.
*/
struct maxHeapNode *extractMin(struct maxHeap *myMaxHeap){

    //get top node which is the one with the lowest value.
    struct maxHeapNode *myNode = myMaxHeap->arrayOfNodes[0];

    //since the first value is getting removed, we save the last value of the array in the first position
    myMaxHeap->arrayOfNodes[0] = myMaxHeap->arrayOfNodes[myMaxHeap->heapSize - 1];

    //lower the size of the heap as a node has been extracted
    --myMaxHeap->heapSize;

    //heapify on the first item.
    maxHeapify(myMaxHeap, 0);

    return myNode;
}


/*
  builds the maxHeap and starts the heapify process.
  takes as input the maxHeap.
*/
void buildMaxHeap(struct maxHeap *myMaxHeap){
    int size = myMaxHeap->heapSize - 1;
    int index;

    for(index = (size - 1)/2; index>= 0; --index){
        maxHeapify(myMaxHeap,index);
    }
}


/*
  inserts a new node in the maxHeap.
  takes as input the maxHeap and the node to insert in the maxHeap.
*/
void insertMaxHeap(struct maxHeap *myMaxHeap, struct maxHeapNode *Node){

    //increase size of the heap to store the new node.
    ++myMaxHeap->heapSize;

    //set the index at the end of the heap.
    int index = myMaxHeap->heapSize - 1;

    /*
      check the lowest position possible to put the new node in order to have an ascending order.
      this allows to put the new node in a spot that makes sense.
    */
    while(index && Node->frequency < myMaxHeap->arrayOfNodes[(index - 1) / 2]->frequency){
        myMaxHeap->arrayOfNodes[index] = myMaxHeap->arrayOfNodes[(index - 1) / 2];
        index = (index - 1) / 2;
    }

    //actually insert the node.
    myMaxHeap->arrayOfNodes[index] = Node;

    /*test made to check the array we'd get for every insert.
    int testNodes[myMaxHeap->heapSize];
    for(int index = 0; index < myMaxHeap->heapSize; ++index){
        testNodes[index] = myMaxHeap->arrayOfNodes[index]->frequency;
    }
    printArray(testNodes,myMaxHeap->heapSize);
    */
}


/*
  checks if the node in question is a leaf or not by checking if any of the children is not NULL
  takes as input a node.
  returns if it's true(1) or false(0).
*/
int isLeaf(struct maxHeapNode *node){
    return (node->leftChild == NULL && node->rightChild == NULL);
}


/*
  creates a new maxHeap with capacity equal to size and insert all values. also starts the heapify process.
  takes as input the array of characters, an array for their frequency and the size of the array.
  returns the maxHeap.
*/
struct maxHeap *createAndBuildMaxHeap(char character[], int frequency[], int size){

    //allocate the memory for the heap.
    struct maxHeap *myMaxHeap = createMaxHeap(size);

    //int freqs[size];

    //allocate memory and create all the nodes of the heap.
    int index;
    for(index = 0; index < size; ++index){
        myMaxHeap->arrayOfNodes[index] = newNode(character[index],frequency[index]);
        //freqs[index] = myMinHeap->arrayOfNodes[index]->frequency;
    }
    //printArray(freqs,size);

    //set the size of the heap and build it.
    myMaxHeap->heapSize = size;
    buildMaxHeap(myMaxHeap);

    return myMaxHeap;
}