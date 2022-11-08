//#include "huffmanUtil.h"
#include "minHeapUtil.h"
#include <stdlib.h>
#include <stdio.h>

#define MAX_TREE_HEIGHT 25

struct minHeapNode *buildHuffmanTree(char data[], int frequency[], int size){
    struct minHeapNode *leftChild, *rightChild, *father;

    struct minHeap *myMinHeap = createAndBuildMinHeap(data, frequency, size);

    while(!isSizeOne(myMinHeap)){
        leftChild = extractMin(myMinHeap);
        rightChild = extractMin(myMinHeap);

        father = newNode('$',leftChild->frequency + rightChild->frequency);
        father->leftChild = leftChild;
        father->rightChild = rightChild;

        insertMinHeap(myMinHeap, father);
    }

    return extractMin(myMinHeap);
}


void printEncoding(struct minHeapNode *node, int code[], int top){
    if(node->leftChild != NULL){
        code[top] = 0;
        printEncoding(node->leftChild, code, top + 1);
    }

    if(node->rightChild != NULL){
        code[top] = 1;
        printEncoding(node->rightChild, code, top + 1);
    }

    if(isLeaf(node)){
        printf("%c: ", node->character);
        printArray(code, top);
    }
}


void huffmanAlgorithm(char data[], int frequency[], int size){
    struct minHeapNode *root = buildHuffmanTree(data, frequency, size);

    int codes[MAX_TREE_HEIGHT], top = 0;
    printEncoding(root, codes, top);
}