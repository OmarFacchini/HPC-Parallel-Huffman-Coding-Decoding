//#include "huffmanUtil.h"
#include "maxHeapUtil.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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


/*encodes a character by traversing the tree recursively until the char is found.
  takes as input the node in which the function currently is, the code of the char that has to be updated based on the movement on the tree
  the output file to write the encoded chars on and the size of the encoding that gets updated every time the code is updated.
*/
void encodeChar(struct maxHeapNode *node, int code[], FILE *myOutput, char readCharacter, int size){
  //check if the node contains the character we want to encode
  if(node->character == readCharacter){
    //if yes write on file the encoded char.
    for(int i = 0; i < size; i++){
      fprintf(myOutput, "%d", code[i]);
    }
    //since the function has done what it had to, return without traversing the entire tree.
    return;
  }else{
    //if the node doesn't containt the character we are wanting to encode, check move on the NON NULL children.
    if(node->leftChild != NULL){
      //going left means adding a 0 to the encoding and recursively call the function.
      code[size] = 0;
      encodeChar(node->leftChild, code, myOutput, readCharacter, size + 1);
    }
    if(node->rightChild != NULL){
      //going right means adding a 1 to the encoding and recursively call the function.
      code[size] = 1;
      encodeChar(node->rightChild, code, myOutput, readCharacter, size + 1);
    }
  }
}


/*reads character from file and encodes it.
  takes as input the node in which the function currently is, the code of the char that has to be updated based on the movement on the tree
  the output file to write the encoded chars on and the size of the encoding that gets updated every time the code is updated.
*/
void readAndEncodeFile(struct maxHeapNode *node, int code[], int size, FILE *myInput, FILE *myOutput){
  //char to store the read char from the file
  char readCharacter = fgetc(myInput);

  //loop until end of file
  while(!feof(myInput)){
    encodeChar(node, code, myOutput, readCharacter, size);

    //read new char
    readCharacter = fgetc(myInput);
  }

}


/*actual call to start the algorithm to build the huffman tree and to prints the encoding obtained.
  takes as input the array of characters, an array for their frequency and the size of the array.
*/
void huffmanAlgorithmEncode(const char *inputFileName, char data[], int frequency[], int size){
  //create the tree
  struct maxHeapNode *root = buildHuffmanTree(data, frequency, size);

  int codes[MAX_TREE_HEIGHT], top = 0;


  //open input and output files.
  FILE *myOutput, *myInput;
  myOutput = fopen("encodedText.txt","w");
  myInput = fopen(inputFileName,"r");

  //check if the files were opened correctly
  if(NULL == myOutput || NULL == myInput){
    printf("file can't be opened or doesn't exist.\n");
    exit(-1);
  }

  readAndEncodeFile(root, codes, 0, myInput, myOutput);

  //printEncoding(root, codes, top, myOutput);

  //close the files.
  fclose(myInput);
  fclose(myOutput);
}
