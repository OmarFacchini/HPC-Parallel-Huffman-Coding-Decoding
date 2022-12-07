//#include "huffmanUtil.h"
#include "maxHeapUtil.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_TREE_HEIGHT 25 //number of letters in the english alphabet, this is temp.


/*
  builds the huffman tree.
  takes as input the array of characters, an array for their frequency and the size of the array.
  returns the root of the tree.
*/
struct maxHeapNode *buildHuffmanTree(char data[], int frequency[], int size){
    //declare three nodes to build a basic binary tree.
    struct maxHeapNode *leftChild, *rightChild, *father;

    //declare the maxHeap.
    struct maxHeap *myMaxHeap = createAndBuildMaxHeap(data, frequency, size);

    //loop until the only node left in is the root.
    while(!isSizeOne(myMaxHeap)){
        //extract the two min values from the heap. 
        leftChild = extractMin(myMaxHeap);
        rightChild = extractMin(myMaxHeap);

        /*
          create a new node that has a special character $ to identify nodes that were created as father of two min nodes.
          the frequency of this node is the sum of the children.
        */
        father = newNode('$',leftChild->frequency + rightChild->frequency);
        father->leftChild = leftChild;
        father->rightChild = rightChild;

        //insert the new node in the heap.
        insertMaxHeap(myMaxHeap, father);
    }

    //extract the root from the heap as it's the only node left.
    return extractMin(myMaxHeap);
}

/*
  builds the huffman dictionary to encode.
  takes as input the root of the huffmanTree, an empty array of char to store the encoding, and empty int to store the length of the encoding.
*/
void buildHuffmanDictionary(struct maxHeapNode *root, char *bitSequence, unsigned int bitSequenceLength){
  //check if leaf has been reached.
  if(root->leftChild == NULL && root->rightChild == NULL){
    //update length of the encoding size.
		myHuffmanDictionary[(int)(root->character - 'a')].bitSequenceLength = bitSequenceLength;
    int i = 0;
    for(i = 0; i < bitSequenceLength; i++){
      //set the memory in the dictionary to store the bitsequence/encoding.
      memset(myHuffmanDictionary[(int)root->character - 97].bitSequence + i, bitSequence[i], sizeof(char));
    }
    return;
	}

  //if node has left child add 0 to path and move left
	if(root->leftChild != NULL){
		bitSequence[bitSequenceLength] = '0';
		buildHuffmanDictionary(root->leftChild, bitSequence, bitSequenceLength + 1);
	}

  //if node has right child add 1 ad move right
	if(root->rightChild != NULL){
		bitSequence[bitSequenceLength] = '1';
		buildHuffmanDictionary(root->rightChild, bitSequence, bitSequenceLength + 1);
	}

}
