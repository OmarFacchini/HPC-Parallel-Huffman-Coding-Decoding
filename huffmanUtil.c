//#include "huffmanUtil.h"
#include "maxHeapUtil.h"
#include <stdlib.h>
#include <stdio.h>

#define MAX_TREE_HEIGHT 25 //number of letters in the english alphabet, this is temp.


/*builds the huffman tree.
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

        /*create a new node that has a special character $ to identify nodes that were created as father of two min nodes.
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


/*stores the encoding of each character in a new file.
  takes as input the array of codes, the index to reach, the character getting encoded on the file and the file itself..
*/
void writeEncodingToFile(int array[], int size, char character, FILE *myFile){

  fprintf(myFile, "%c: ", character);

  int index;
  for(index = 0; index < size; ++index){
      fprintf(myFile,"%d", array[index]);
  }
  fprintf(myFile,"\n");

}


/*prints the encoding of each character
  takes as input the root of the tree, the array of codes an index and a file to store the encodings on.
*/
void printEncoding(struct maxHeapNode *node, int code[], int top, FILE *myFile){

    //set the code to 0 for moving left.
    if(node->leftChild != NULL){
        code[top] = 0;
        printEncoding(node->leftChild, code, top + 1, myFile);
    }

    //set the code to 1 for moving right.
    if(node->rightChild != NULL){
        code[top] = 1;
        printEncoding(node->rightChild, code, top + 1, myFile);
    }

    //if the node is a leaf a character has been reached to the whole encoding is printed.
    if(isLeaf(node)){
        printf("%c: ", node->character);
        printArray(code, top);

        writeEncodingToFile(code, top, node->character, myFile);
    }
}

/*actual call to start the algorithm to build the huffman tree and to prints the encoding obtained.
  takes as input the array of characters, an array for their frequency and the size of the array.
*/
void huffmanAlgorithm(char data[], int frequency[], int size){
    struct maxHeapNode *root = buildHuffmanTree(data, frequency, size);

    int codes[MAX_TREE_HEIGHT], top = 0;
    FILE *myFile;
    myFile = fopen("encoding.txt","w");
    printEncoding(root, codes, top, myFile);
    fclose(myFile);
}
