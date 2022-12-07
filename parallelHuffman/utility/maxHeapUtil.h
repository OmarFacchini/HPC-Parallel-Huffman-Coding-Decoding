#ifndef maxHeapUtil_H   /* Include guard */
#define maxHeapUtil_H

struct huffmanDictionary{
	char bitSequence[27];
	unsigned int bitSequenceLength;
};

extern struct huffmanDictionary myHuffmanDictionary[26];

//structure that represents a node in the maxHeap.
struct maxHeapNode{
    //character contained in the node
    char character;

    //frequency with which the character appears
    //used as character that appear ofter will be encoded with a lower amount of bits compared to rare characters
    unsigned int frequency;

    //left and right child of the node in the "tree"
    struct maxHeapNode *leftChild, *rightChild;
};


//structure that represents the maxHeap.
struct maxHeap{
    //size of the maxheap;
    unsigned int heapSize;

    //max amount of items acceptable.
    unsigned int capacity;

    //array of the nodes that form the heap;
    struct maxHeapNode **arrayOfNodes;
};


/*
  allocates memory for a new node.
  takes as input the character to store in the node and the frequency with which the character appears in the file.
  returns the newly created node.
*/
struct maxHeapNode *newNode(char character, unsigned int frequency);


/*
  allocates memory for maxHeap of a given capacity.
  takes as input the capacity which is the max amount of items acceptable. 
  returns the newly created maxHeap.
*/
struct maxHeap *createMaxHeap(unsigned int capacity);


/*
  swaps two nodes.
  takes as input the pointer to two nodes and swaps them.
  used in the maxHeapify to orded.
*/
void swapNodes(struct maxHeapNode **higherNode, struct maxHeapNode **lowerNode);


/*
  standard maxHeapify function, used to order items in the heap in order to have the min value as the first item.
  takes as input the maxHeap and the index of the item so heapify on.
*/
void maxHeapify(struct maxHeap *myMaxHeap, unsigned int index);


/*
  checks if the maxHeap is of size 1.
  takes as input the maxHeap.
  returns if it's true(1) or false(0).
*/
int isSizeOne(struct maxHeap *myMaxHeap);


/*
  extracts the min value from the heap and heapify.
  takes as input the maxHeap.
  returns the node with the lowest value.
*/
struct maxHeapNode *extractMin(struct maxHeap *myMaxHeap);


/*
  builds the maxHeap and starts the heapify process.
  takes as input the maxHeap.
*/
void buildMaxHeap(struct maxHeap *myMaxHeap);


/*
  inserts a new node in the maxHeap.
  takes as input the maxHeap and the node to insert in the maxHeap.
*/
void insertMaxHeap(struct maxHeap *myMaxHeap, struct maxHeapNode *Node);


/*
  checks if the node in question is a leaf or not by checking if any of the children is not NULL
  takes as input a node.
  returns if it's true(1) or false(0).
*/
int isLeaf(struct maxHeapNode *node);


/*
  creates a new maxHeap with capacity equal to size and insert all values. also starts the heapify process.
  takes as input the array of characters, an array for their frequency and the size of the array.
  returns the maxHeap.
*/
struct maxHeap *createAndBuildMaxHeap(char character[], int frequency[], int size);


#endif