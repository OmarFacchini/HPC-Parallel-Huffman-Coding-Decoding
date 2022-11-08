#ifndef huffmanUtil_H   /* Include guard */
#define huffmanUtil_H


#include "maxHeapUtil.h"


/*builds the huffman tree.
  takes as input the array of characters, an array for their frequency and the size of the array.
  returns the root of the tree.
*/
struct maxHeapNode *buildHuffmanTree(char data[], int frequency[], int size);


/*prints the encoding of each character
  takes as input the root of the tree, the array of codes and an index.
*/
void printEncoding(struct maxHeapNode *node, int code[], int top);


/*actual call to start the algorithm to build the huffman tree and to prints the encoding obtained.
  takes as input the array of characters, an array for their frequency and the size of the array.
*/
void huffmanAlgorithm(char data[], int frequency[], int size);

#endif