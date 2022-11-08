#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "maxHeapUtil.h"
#include "huffmanUtil.h"

struct charAppearance{
    char character;
    int frequency;
};


int main(int argc, char const *argv[]){

    char arr[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
    int freq[] = { 5, 9, 12, 13, 16, 45 };
 
    int size = sizeof(arr) / sizeof(arr[0]);
    //printf("size: %d\nsizeof(arr): %d\nsizeof(arr[0]): %d\n", size, sizeof(arr), sizeof(arr[0]));
    huffmanAlgorithm(arr, freq, size);
 
    return 0;
}




/*int main(int argc, char const *argv[]){

    //check compilation gives the right amount of items
    /*if(argc != 2){
        printf("wrong usage");
        return -1;
    }*/

    /*FILE *myFile;
    char readChar;

    //declare file to open in read mode.
    //if pointer to file is null the file doesn't exist or can't be opened
    myFile = fopen(argv[1],"r");
    if(NULL == myFile){
        printf("file can't be opened or doesn't exist.\n");
    }
    

   do{
        readChar = fgetc(myFile);

   }while(readChar != EOF);

    //close the file
    fclose(myFile);

}*/