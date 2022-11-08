#ifndef minHeapUtil_H   /* Include guard */
#define minHeapUtil_H

struct minHeapNode{
    //character contained in the node
    char character;

    //frequency with which the character appears
    //used as character that appear ofter will be encoded with a lower amount of bits compared to rare characters
    unsigned int frequency;

    //left and right child of the node in the "tree"
    struct minHeapNode *leftChild, *rightChild;
};


struct minHeap{
    //size of the minheap;
    unsigned int heapSize;

    //max amount of items acceptable.
    unsigned int capacity;

    //array of the nodes that form the heap;
    struct minHeapNode **arrayOfNodes;
};

//function that creates a new node.
struct minHeapNode *newNode(char character, unsigned int frequency);

struct minHeap *createMinHeap(unsigned int capacity);

void swapNodes(struct minHeapNode **higherNode, struct minHeapNode **lowerNode);

void minHeapify(struct minHeap *myMinHeap, unsigned int index);

int isSizeOne(struct minHeap *myMinHeap);

//not sure if it's get min or extract min
struct minHeapNode *extractMin(struct minHeap *myMinHeap);

void buildMinHeap(struct minHeap *myMinHeap);

void insertMinHeap(struct minHeap *myMinHeap, struct minHeapNode *Node);

void printArray(int array[], int size);

int isLeaf(struct minHeapNode *node);

struct minHeap *createAndBuildMinHeap(char character[], int frequency[], int size);


#endif