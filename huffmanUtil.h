#ifndef huffmanUtil_H   /* Include guard */
#define huffmanUtil_H


#include "maxHeapUtil.h"


/*builds the huffman tree.
  takes as input the array of characters, an array for their frequency and the size of the array.
  returns the root of the tree.
*/
struct maxHeapNode *buildHuffmanTree(char data[], int frequency[], int size);


/*prints the encoding of each character
  takes as input the root of the tree, the array of codes an index and a file to store the encodings on.
*/
void printEncoding(struct maxHeapNode *node, int code[], int top, FILE *myFile);


/*actual call to start the algorithm to build the huffman tree and to prints the encoding obtained.
  takes as input the array of characters, an array for their frequency and the size of the array.
*/
void huffmanAlgorithmEncode(const char *inputFileName, char data[], int frequency[], int size);


/*stores the encoding of each character in a new file.
  takes as input the array of codes, the index to reach, the character getting encoded on the file and the file itself..
*/
void writeEncodingToFile(int array[], int size, char character, FILE *myFile);

#endif