#ifndef huffmanUtil_H   /* Include guard */
#define huffmanUtil_H


#include "maxHeapUtil.h"
#include <stdio.h>


/*
  builds the huffman tree.
  takes as input the array of characters, an array for their frequency and the size of the array.
  returns the root of the tree.
*/
struct maxHeapNode *buildHuffmanTree(char data[], int frequency[], int size);


/*
  builds the huffman dictionary to encode.
  takes as input the root of the huffmanTree, an empty array of char to store the encoding, and empty int to store the length of the encoding.
*/
void buildHuffmanDictionary(struct maxHeapNode *root, char *bitSequence, unsigned int bitSequenceLength);

#endif