#ifndef charFrequencyUtil_H   /* Include guard */
#define charFrequencyUtil_H

/*preprocesses the file to encode.
  takes as input the file name, an empty frequency array and a reference to how many lines the file has, this is calculated in the function.
  returns the array of frequencies for each letter.
*/
int *filePreprocessing(const char *fileName, int *frequency, int *rowsInFile);

#endif