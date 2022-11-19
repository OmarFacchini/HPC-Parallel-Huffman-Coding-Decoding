#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "maxHeapUtil.h"
#include "huffmanUtil.h"
#include "charFrequencyUtil.h"


int main(int argc, char const *argv[]){

    if(argc != 3){
        printf("wrong usage, try using:\n.\\huffman.exe .\\input.txt decode\nor\n.\\huffman.exe .\\input.txt encode");
        return -1;
    }

    //get the name of the file.
    const char *myFile = argv[1];

    //char arr[] = { 'a', 'b', 'c', 'd', 'e', 'f' };
    //int freq[] = { 5, 9, 12, 13, 16, 45 };

    //array of chars in the english alphabet.
    char arr[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

    //number of elements in the array.
    int size = sizeof(arr) / sizeof(arr[0]);

    //allocate array for frequency of each char.
    int *freq = (int*)malloc(sizeof(arr) * sizeof(int)); 
    
    //check if the allocation of the array was successful, else end the program.
    if(freq == NULL){
        printf("couldn't allocate memory for frequency array");
        return -1;
    }
    
    if(strcmp(argv[2],"encode") == 0){
        //set all the values in the array to be 0 as at the start not a single char has been seen yet.
        for(int i = 0; i < 26; i++){
                freq[i] = 0;
        }

        //count rows in the file.
        int totalRowsInFile = 0;

        //process the file to get frequency of chars and number of rows in the file.
        freq = filePreprocessing(myFile, freq, &totalRowsInFile);
        //printf("frequency of a: %d\n",freq[0]);

        //store the frequency of the words in the encoded file.
        storeFrequencyOnFile(freq, arr, size);

        //printf("size: %d\nsizeof(arr): %d\nsizeof(arr[0]): %d\n", size, sizeof(arr), sizeof(arr[0]));
        huffmanAlgorithmEncode(myFile, arr, freq, size);
    }

    if(strcmp(argv[2],"decode") == 0){
    }

    free(freq);
 
    return 0;
}
