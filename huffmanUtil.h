#ifndef huffmanUtil_H   /* Include guard */
#define huffmanUtil_H

#include "minHeapUtil.h"

struct minHeapNode *buildHuffmanTree(char data[], int frequency[], int size);

void printEncoding(struct minHeapNode *node, int code[], int top);

void huffmanAlgorithm(char data[], int frequency[], int size);

#endif