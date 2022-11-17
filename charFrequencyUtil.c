#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


/*preprocesses the file to encode.
  takes as input the file name, an empty frequency array and a reference to how many lines the file has, this is calculated in the function.
  returns the array of frequencies for each letter.
*/
int *filePreprocessing(const char *fileName, int *frequency, int *rowsInFile){
  
  //declare file to read data from.
  FILE *myFile;

  //open the file in read mode, file name is given as input in the cli.
  myFile = fopen(fileName,"r");
  
  //myFile = fopen(strcat(fileName,".txt"),"r"); this row only wants the name without extension.

  //check if the file was opened correctly
  if(NULL == myFile){
      printf("file can't be opened or doesn't exist.\n");
      exit(-1);
  }

  //char to store the read char from the file
  char readCharacter;

  //read character
  readCharacter = fgetc(myFile);

  int index;

  //loop until end of file
  while(!feof(myFile)){
    /*rowsInFile is passed by reference and stores how many rows are in a file
      this will be used in the parallel application to know how many rows each process/thread has to read.
    */
    if((int)readCharacter == 10){
        (*rowsInFile)++;
    }

    //check if char is in uppercase and if so put it lower.
    if((int)readCharacter >= 65 && (int)readCharacter <= 90){
      //lowercase the character by adding the difference 'a'-'A'(spoiler: it's 32).
      readCharacter = readCharacter + 32;
    }

    //check if char is in lowercase.
  if((int)readCharacter >= 97 && (int)readCharacter <= 122){
    /*normalize the index to make sure 'a' equals to the first index.
      and increase the frequency of the character whenever it's seen.
    */
    index = (int)readCharacter - 'a';
    frequency[index]++;
  }

    // printf("readchar: %c\n(int)readChar: %d\n(int)readCharacter - 'a': %d\nindex: %d\nrows: %d\n\n",
    //         readCharacter,(int)readCharacter,(int)readCharacter - 'a',index,*rowsInFile);
    
    //read character
    readCharacter = fgetc(myFile);
  }

  //if the file is empty i get 0 rows, otherwise as the last value read in the file is eof and not \n an additional row has to be added
  if((*rowsInFile) > 0){
    (*rowsInFile)++;
  }

  //close the file.
  fclose(myFile);

  //return the frequency array.
  return frequency;
}
