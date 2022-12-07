#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


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

    //array of chars in the english alphabet.
    char arr[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

    //number of elements in the array.
    int size = sizeof(arr) / sizeof(arr[0]);

    //allocate array for frequency of each char.
    int *freq = (int*)malloc(sizeof(arr) * sizeof(int)); 

    //count rows in the file.
    int totalRowsInFile = 0;
    
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

        FILE *myCSV;
        //remember to change the name of the csv file based on the input file.
        myCSV = fopen(".\\results\\serialResults\\10000000wordsDense.csv", "w");
        fprintf(myCSV, "fileProcessingTime, encodingTime, totalTime\n");

        clock_t start, end;
        double fileProcessingTime, totalTime, encodingTime;

        //run 100 times just to have an accurate average.
        for(int i = 0; i < 100; i++){

            start = clock();

            //process the file to get frequency of chars and number of rows in the file.
            freq = filePreprocessing(myFile, freq, &totalRowsInFile);

            //store the frequency of the words in the encoded file.
            storeFrequencyOnFile(freq, arr, size);

            end = clock();
            fileProcessingTime = (double)((end - start)) / CLOCKS_PER_SEC;
            //printf("time taken for file processing: %.3f\n", fileProcessingTime);



            //printf("size: %d\nsizeof(arr): %d\nsizeof(arr[0]): %d\n", size, sizeof(arr), sizeof(arr[0]));
            huffmanAlgorithmEncode(myFile, arr, freq, size);

            end = clock();
            totalTime = (double)((end - start)) / CLOCKS_PER_SEC;
            //printf("Total time taken: %.3f\n", totalTime);

            encodingTime = totalTime - fileProcessingTime;
            //printf("time taken for encoding: %.3f\n\n", encodingTime);

            fprintf(myCSV, "%.3f, %.3f, %.3f\n", fileProcessingTime,encodingTime,totalTime);

        }

        fclose(myCSV);
    }

    if(strcmp(argv[2],"decode") == 0){

        //declare file to read data from.
        FILE *myInput;

        //open the file in read mode, file name is given as input in the cli.
        myInput = fopen(myFile,"r");

        //check if file exists.
        if(NULL == myInput){
            printf("file can't be opened or doesn't exist.\n");
            exit(-1);
        }

        freq = getFrequencyFromFile(myInput, freq);

        /*for(int i = 0; i < 26; i++){
                printf("%d\n",freq[i]);
        }*/

        huffmanAlgorithmDecode(myInput, arr, freq, size);

        //close file stream.
        fclose(myInput);

    }

    free(freq);
 
    return 0;
}
