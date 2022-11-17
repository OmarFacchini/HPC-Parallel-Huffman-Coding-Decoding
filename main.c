#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "maxHeapUtil.h"
#include "huffmanUtil.h"
#include "charFrequencyUtil.h"


int main(int argc, char const *argv[]){

    if(argc != 2){
        printf("wrong usage\ntry using .\\huffman.exe input.txt");
        return -1;
    }

    //char arr[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
    //int freq[] = { 5, 9, 12, 13, 16, 45 };

    //array of chars in the english alphabet.
    char arr[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

    //allocate array for frequency of each char.
    int *freq = (int*)malloc(sizeof(arr) * sizeof(int)); 
    
    //check if the allocation of the array was successful, else end the program.
    if(freq == NULL){
        printf("couldn't allocate memory for frequency array");
        return -1;
    }

    //set all the values in the array to be 0 as at the start not a single char has been seen yet.
    for(int i = 0; i < 26; i++){
         freq[i] = 0;
    }

    //get the name of the file.
    const char *myFile = argv[1];

    //count rows in the file.
    int totalRowsInFile = 0;

    //process the file to get frequency of chars and number of rows in the file.
    freq = filePreprocessing(myFile, freq, &totalRowsInFile);
    //printf("frequency of a: %d\n",freq[0]);
    
 
    int size = sizeof(arr) / sizeof(arr[0]);
    //printf("size: %d\nsizeof(arr): %d\nsizeof(arr[0]): %d\n", size, sizeof(arr), sizeof(arr[0]));
    huffmanAlgorithm(arr, freq, size);

    free(freq);
 
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