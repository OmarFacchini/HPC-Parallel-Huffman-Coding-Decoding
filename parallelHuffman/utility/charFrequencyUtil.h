#ifndef charFrequencyUtil_H   /* Include guard */
#define charFrequencyUtil_H
#include <stdio.h>

/*
  preprocesses the file to encode.
  takes as input the file name, an empty frequency array and a reference to how many lines the file has, this is calculated in the function.
  returns the array of frequencies for each letter.
*/
int *filePreprocessing(const char *fileName, int *frequency, int *rowsInFile);


/*
  after the processing write the frequency and the chars in the file to be able to decode it later.
  takes as input the file name, the array with the frequency of each character, the array of characters and the size of the array.
*/
void storeFrequencyOnFile(const char *fileName, int *frequency, char *characters, int size);


/*
  reads the frequency of each character from the file and stores it in orded to build the tree to decode.
  takes as input the file name and an empty frequency array.
  returns the array of frequencies for each letter.
*/
int *getFrequencyFromFile(FILE *myFile, int *frequency);

#endif