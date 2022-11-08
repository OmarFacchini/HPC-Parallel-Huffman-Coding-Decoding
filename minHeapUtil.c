#include "minHeapUtil.h"
#include <stdlib.h>
#include <stdio.h>

//creates a new node.
struct minHeapNode *newNode(char character, unsigned int frequency){

    //allocate memory for a new node.
    struct minHeapNode *node = (struct minHeapNode *)malloc(sizeof(struct minHeapNode));

    //set both children to NULL as the node at the moment is a leaf in the tree.
    node->leftChild = node->rightChild = NULL;

    //set the character and the frequency.
    node->character = character;
    node->frequency = frequency;

    //return the node.
    return node;
}

//creates a minHeap of a given capacity.
struct minHeap *createMinHeap(unsigned int capacity){

    //allocate memory for a new minHeap
    struct minHeap *newMinHeap = (struct minHeap *)malloc(sizeof(struct minHeap));
    
    //per definition a new minHeap is always empty, which means of size 0.
    newMinHeap->heapSize = 0;

    //set the heap capacity
    newMinHeap->capacity = capacity;

    //allocate memory for the array of nodes with size equal to the [max amount of items(capacity) *the size of a node]
    newMinHeap->arrayOfNodes = (struct minHeapNode **)malloc(newMinHeap->capacity * sizeof(struct minHeapNode *));
    return newMinHeap;
}

//swaps two nodes.
void swapNodes(struct minHeapNode **higherNode, struct minHeapNode **lowerNode){
    struct minHeapNode *temp = *higherNode;
    *higherNode = *lowerNode;
    *lowerNode = temp;
}

//standard minHeapify
void minHeapify(struct minHeap *myMinHeap, unsigned int index){
    //take index of smallest value and both the children
    int smallest = index;
    int leftChild  = 2 * index + 1;
    int rightChild = 2 * index + 2;
    
    //check if the left child's value is lower than the smallest value, if so the left child is the smallest
    if(leftChild < myMinHeap->heapSize && 
       myMinHeap->arrayOfNodes[leftChild]->frequency < myMinHeap->arrayOfNodes[smallest]->frequency){
        smallest = leftChild;
       }
    
    //same as before but for the right child
    if(rightChild < myMinHeap->heapSize &&
       myMinHeap->arrayOfNodes[rightChild]->frequency < myMinHeap->arrayOfNodes[smallest]->frequency){
        smallest = rightChild;
       }
    
    //if the smallest has been updated than we have to heapify again.
    if(smallest != index){
        swapNodes(&myMinHeap->arrayOfNodes[smallest],&myMinHeap->arrayOfNodes[index]);
        minHeapify(myMinHeap,smallest);
    }

}

//checks if the heap size is 1
int isSizeOne(struct minHeap *myMinHeap){
    return (myMinHeap->heapSize == 1);
}

//extracts the min value from the heap
struct minHeapNode *extractMin(struct minHeap *myMinHeap){

    //get top node
    struct minHeapNode *myNode = myMinHeap->arrayOfNodes[0];
    myMinHeap->arrayOfNodes[0] = myMinHeap->arrayOfNodes[myMinHeap->heapSize - 1];

    --myMinHeap->heapSize;
    minHeapify(myMinHeap, 0);

    return myNode;
}

//builds the minHeap
void buildMinHeap(struct minHeap *myMinHeap){
    int size = myMinHeap->heapSize - 1;
    int index;

    for(index = (size -1)/2; index>= 0; --index){
        minHeapify(myMinHeap,index);
    }
}

//inserts a new node in the minHeap
void insertMinHeap(struct minHeap *myMinHeap, struct minHeapNode *Node){
    ++myMinHeap->heapSize;
    int index = myMinHeap->heapSize - 1;

    while(index && Node->frequency < myMinHeap->arrayOfNodes[(index - 1) / 2]->frequency){
        myMinHeap->arrayOfNodes[index] = myMinHeap->arrayOfNodes[(index - 1) / 2];
        index = (index - 1) / 2;
    }

    myMinHeap->arrayOfNodes[index] = Node;
}

//prints the tree built, used only for testing, not really useful
void printArray(int array[], int size){
    int index;
    for(index = 0; index < size; ++index){
        printf("%d",array[index]);
    }
    printf("\n");
}

//checks if the node in question is a leaf or not by checking if any of the children is not NULL
int isLeaf(struct minHeapNode *node){
    return (node->leftChild == NULL && node->rightChild == NULL);
}

//creates a new minHeap with capacity equal to size and insert all values.
struct minHeap *createAndBuildMinHeap(char character[], int frequency[], int size){
    struct minHeap *myMinHeap = createMinHeap(size);

    for(int index = 0; index < size; ++index){
        myMinHeap->arrayOfNodes[index] = newNode(character[index],frequency[index]);
    }

    myMinHeap->heapSize = size;
    buildMinHeap(myMinHeap);

    return myMinHeap;
}