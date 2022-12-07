#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

#include "./utility/maxHeapUtil.h"
#include "./utility/huffmanUtil.h"
#include "./utility/charFrequencyUtil.h"

//global declaration of a dictionary, size is 26 as we are only encoding the english alphabet chars.
struct huffmanDictionary myHuffmanDictionary[26];

int main(int argc, char *argv[]){

    //init the dictionary memory with all dots.
    memset(myHuffmanDictionary, '.', 26 * sizeof(struct huffmanDictionary));

    //init MPI environment
    MPI_Init(&argc, &argv);

    //get number of processes in the communicator.
    int numberOfProcesses;
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);

    //get rank of the process.
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    //get processor name, not used in the final product but interesting to see the parallelism
    //as this allows to see on what processor each process is run.
    char processorName[MPI_MAX_PROCESSOR_NAME];
    int nameLength;
    MPI_Get_processor_name(processorName, &nameLength);

    //timing variables.
    clock_t start, end, globalStart, globalEnd;
    double fileProcessingTime, totalTime, encodingTime;
    
    //start clock for total time of the process.
    globalStart = clock();

    //get the name of the input file.
    const char *myFile = argv[1];

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

    //set all the values in the array to be 0 as at the start not a single char has been seen yet.
    int i;
    for(i = 0; i < 26; i++){
            freq[i] = 0;
    }



    //here are public declaration as each process need these variables in local.

    //rowsInFile is not really used as this should've been used to decide the amount of data each process had to read
    //but a better approach has been chosen with bytes number.
    int rowsInFile = 0;

    /*
      inputFileLength is how many bytes the file has.
      blockSize is how many bytes each process has to encode.
      *encodedBlocksArray is an array of how many bytes resulted from the encoding for each process.
    */
    unsigned int inputFileLength = 0, blockSize = 0, *encodedBlocksArray;

    /* 
       *inputData is the array that contains the whole original file text.
       *encodedData is the array that containts the encoded text.
       bitSequenceLength is used as an index for the contruction of the huffmanDictionary.
    */
    unsigned char *inputData, *encodedData, bitSequenceLength = 0;

    /* 
       as we have the english alphabet to encode the max length of the encoded text is 26, 27 is used in order to be able to add the terminator '\0' 
       this is empty and is used in the construction of the huffmanDictionary.
    */
    char bitSequence[27];

    /*
       root of the binary tree, used to generate the sequence of bits to encode each character.
       mostly used to build the huffmanDictionary.
    */
    struct maxHeapNode *root;

    /* 
        j is a normal for loop variable.

        encodedDataSize is used to know the size of the encoded data. in this application is not really used as the path obtained from the tree
        represents bits and not chars, chars can be used to check the correctness of the encoding and if each '0' or '1' was to be considered a char we would have
        the endodedDataSize grow and the encodedData array would need reallocs as the encoding size for each letter would be pathLength*charSize.
        but since we use bits this will never grow, would never be even reached actually ideally, hence why it's not used.

        extraBytes similar to the encodedDataSize is not used as this would be the amount of bytes the realloc should add to the encodedData array.

        encodedBytes similar to the past two variables was used for testing to keep track of the amount of bytes that had been encoded up to that point.

        encodeIndex is the index used to keep track of where in the encodedData array we are inserting.

        bitsFilled is used to keep track of how many bits have been filled in the byte to make sure to have encodedData[encodeIndex] be a full byte.

    */
    int j, encodedDataSize = 0, extraBytes = 0, encodedBytes = 0, encodeIndex = 0, bitsFilled = 0;

    //this is the value of the bits that fill the byte.
    unsigned char bits = 0;

    //these are mpi files as this opens the file for all the processes meaning each process has its own handle to the file avoiding conflicts.
    MPI_File mpi_inputFile, mpi_compressedFile;
	MPI_Status status;

    //only rank 0 prints the parameters.
    if(rank == 0){
        printf("process, fileProcessingTime, encodingTime, totalTime\n");
    }

    //start the fileProcessing time count.
    start = clock();

    //process the file to get frequency of chars and number of rows in the file.
    freq = filePreprocessing(argv[1], freq, &rowsInFile);

    /*
      store frequency on a file as it's what will be used to decode.
      only done by process 0 as every process works on the same data.
    */
    if(rank == 0){
        storeFrequencyOnFile("/home/omar.facchini/HPC-Parallel-Huffman-Coding-Decoding/parallel/wordFrequency", freq, arr, size); 
    }

    //get how much time passed for the fileProcessing time count.
    end = clock();

    //expect fileprocessingtime to be a bit higher on process 0 as it also writes the frequency
    fileProcessingTime = (double)((end - start)) / CLOCKS_PER_SEC;

    /*
       build tree and get the root using maxHeap so the most common characters have a shorter path, as we wanted.
       only get the root as from there we can navigate through the whole tree.

       this is done by every process locally because even if redundant there is almost no difference in time while having no communications.
       While it would be theoretically better to have only process 0 build the tree and broadcast it, we should broadcast a whole piece of the heap 
       which would take way too long and based on the tree could saturate the bandwidth.
       Calculating it locally takes a bit of time but not as much as broadcasting it hence why this was the chosen approach.

    */
    root = buildHuffmanTree(arr, freq, size);

    /*
       opposed to the building of the HuffmanTree, here we are calculating only how many bytes the txt file to encode has
       and it would be no problem to broadcast it to every process so we have only one process do this.

       could also have each process get the length by its own but to avoid file access conflicts(there shouldn't be as it's only reads)
       and as it's faster to broadcast a value than to go through the whole file n times, the above approach has been chosen.
    */
    if(rank == 0){
        FILE *inputFile;

        //open the file in read.
        inputFile = fopen(argv[1], "r");

        //set pointer in file to the end.
		fseek(inputFile, 0, SEEK_END);

        //as the pointer is set at the end, we get the length of the file
		inputFileLength = ftell(inputFile);

        //set the pointer to the beginning of the file.
		fseek(inputFile, 0, SEEK_SET);

        //close file.
		fclose(inputFile);
    }


    /*
       broadcast size of file to all the processes from process 0. 
       no need to sync as bcast is thread-safe hence every process will wait to have received this information to proceed.
       input, elements in buffer, type of elements in buffer, root process, communicator.
    */
	MPI_Bcast(&inputFileLength, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);


    /*
        THIS WAS DONE FOR THE APPROACH IN WHICH ONLY ONE PROCESS WOULD BUILD THE TREE AND BROADCAST IT.
        COMMENTED AS THIS APPROACH HAS BEEN DISCARDED.
        KEPT TO SHOW HOW A DIFFERENT APPROACH COULD'VE BEEN DONE.

        //create mpi data structure to send my node.
        //not really sure if this works due to the recursiveness.

        const int numberOfItems = 4;
        int blockLengths[4] = {1,1,1,1};
        MPI_Datatype MPI_maxHeapNode;
        MPI_Datatype types[4] = {MPI_CHAR, MPI_UNSIGNED, MPI_maxHeapNode, MPI_maxHeapNode};
        MPI_Aint offsets[4];
        offsets[0] = offsetof(struct maxHeapNode, character);
        offsets[1] = offsetof(struct maxHeapNode, frequency);
        offsets[2] = offsetof(struct maxHeapNode, leftChild);
        offsets[3] = offsetof(struct maxHeapNode, rightChild);
        

        MPI_Type_create_struct(numberOfItems, blockLengths, offsets, types, &MPI_maxHeapNode);
        MPI_Type_commit(&MPI_maxHeapNode);

        //broadcast the root to all the processes.
        MPI_Bcast(root, 1, MPI_maxHeapNode, 0, MPI_COMM_WORLD);

    */

    //get the size of the block each process has to work on, basically how many bytes each process has to encode.
    blockSize = inputFileLength / numberOfProcesses;

    /*
       as blocksize is an int the division might not be precise and we round to the lower value(C standard)
       so the last block has to compensate for that.
       to better understand use rank=4, numberOfProcesses=5, inputFileLength=102 and calculate general blockSize and the last one.
    */
    if(rank == (numberOfProcesses -1)){
        //this will differ locally only in the last process for all the others it will be the same value.
        blockSize = inputFileLength - ((numberOfProcesses -1) * blockSize);
    }


    /*
       the next block reads the file and stores the data in the inputData array.
       this is done by each process by setting the pointer in the file to where the process has to start reading from and reads the blockSize assigned to it.
       as each process has its own file handler and MPI takes care of the access conflicts this allows to have only the data each process will work on in its memory
       and avoids broadcasts hence why this approach has been chosen.

       another approach possible would've been to have a process (ex: process 0) read and store the whole data and use the 
       MPI_Scatter(inputData, blockSize, MPI_UNSIGNED_CHAR, localInputDataArray, blockSize, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
       function to send to each process the needed data.
       note that this brings a problem on the last process as the blocksize might be bigger due do what explained in the last function
       as the function is blocking there would be no sync problems.

       as the only part where the syncing would be required is much later a non blocking approach could be used by using the MPI_scatter() function
       and putting a barrier right before the section where we need to be sure each process has the inputData.
    */

    //start a new fileProcessing timer to add to the old one.
    start = clock();

    //open file in each process, read amount of data equal to blockSize.
	MPI_File_open(MPI_COMM_WORLD, argv[1], MPI_MODE_RDONLY, MPI_INFO_NULL, &mpi_inputFile);

    //set pointer to an offset for each process as each process has to read only a part of the file.
	MPI_File_seek(mpi_inputFile, rank * blockSize, MPI_SEEK_SET);

    //allocate memory in each process to store the data from the file.
    inputData = (unsigned char *)malloc(blockSize * sizeof(unsigned char));

    //read blockSize data from the file and store it in inputData.
    MPI_File_read(mpi_inputFile, inputData, blockSize, MPI_UNSIGNED_CHAR, &status);

    /*
        close the file handler.
        no need for barrier to sync as each process has its own handler, so this closes the file only for the single process.
    */
    MPI_File_close(&mpi_inputFile);

    //stop fileProcessing timer and add to old one as this is still a fileProcessing task.
    end = clock();
    fileProcessingTime = fileProcessingTime + ((double)((end - start)) / CLOCKS_PER_SEC);

    //allocate memory for array containing the size of each encoded block for every process.
    encodedBlocksArray = (unsigned int *)malloc(numberOfProcesses * sizeof(unsigned int));

    /*
       allocate memory for array containing the encoded data, as the purpose of encoding is also to compress
       the size cannot be bigger than the original size of data that the process has to work on.
       using this size also allows to make sure we compressed the data.
    */
    encodedData = (unsigned char *)malloc(blockSize * sizeof(unsigned char));

    /*
       set the size of the block for this process to 0, no point doing it for every other process as 
       each process will only access its own variable.
       this must be an array as it has to store this information for every process.
    */
    encodedBlocksArray[rank] = 0;

    /*
       like for the building of the HuffmanTree, we build the dictionary locally for each process.
    */
    buildHuffmanDictionary(root, bitSequence, 0);
    /*
        //just a test that prints the whole dictionary to make sure the building was successful
        //only one process prints as the dictionary is the same for every process so no point printing it for everyone.
        if(rank == 0){
            int test= 0;
            buildHuffmanDictionary(root, bitSequence, 0);
            //it works!!!!!!!!!!
            for(test = 0; test < 26; test++){
                printf("code: ");
                for(i = 0; i < myHuffmanDictionary[test].bitSequenceLength; i++){
                    printf("%c",myHuffmanDictionary[test].bitSequence[i]);

                }
                printf("\tchar: %c\tlength: %u\n\n",'a'+ test, myHuffmanDictionary[test].bitSequenceLength);
            }
        }
    */


    //sizeof is compile-time, so it's constant and can't be used to keep track of the memory allocated we need to do it ourselves.
    encodedDataSize = blockSize * sizeof(unsigned char);

    //start timer for encodingTime
    start = clock();

    for(i = 0; i < blockSize; i++){
        //if there is a special character, just store it.
        if((int)inputData[i] < 'A' || (int)inputData[i] > 'z' || ((int)inputData[i] > 'Z' && (int)inputData[i] < 'a')){
            //at the moment it's all commented as the test cases were run on basic random words generated from the english alphabet and are only formed
            //by random letters, this is here in case in future random characters were to be added.
            /*
                //for the situation where each value of the path is considered a char and need realloc.
                //check if the array is big enough to store another char, otherwise add another byte.
                if(encodedBytes + 1 >= encodedDataSize){
                    encodedData = (unsigned char *)realloc(encodedData, encodedDataSize + 1);
                    encodedDataSize++;
                    printf("i: %d\treallocated to: %d\n",i,encodedDataSize);
                }
            */

            // encodedBytes++;

            //insert char in the next index(as the current one might be already filled with some values)
            //encodedData[encodeIndex+1] = inputData[i]; 
            //printf("encodedData: %c\n", encodedData[encodeIndex+1]);
            //reset the bits values and increment by 2 the index(1 for the access to the current byte and 1 more to move on the next free one)
            //bits = 0;
            //bitsFilled = 0;
            //encodeIndex = encodeIndex + 2;

        }else{
            //check if char is in uppercase and if so put it lower
            if((int)inputData[i] >= 'A' && (int)inputData[i] <= 'Z'){
                inputData[i] = inputData[i] + 32;
            }

            /*
                //calculate extra bytes to add in memory to store the encoding.
                //divide by 8 as 1 byte is 8 bits.
                extraBytes = myHuffmanDictionary[(int)inputData[i] - 'a'].bitSequenceLength / 8;

                //check if have to add one more as bits might be 9 and we would add only 1 byte if we dont consider the module
                if(myHuffmanDictionary[(int)inputData[i] - 'a'].bitSequenceLength % 8 >= 1){
                    extraBytes++;
                }

                //if i dont have space left
                if(encodedBytes + extraBytes >= encodedDataSize){
                    //realloc to increase the encodingData accordingly to the new encodings we add.
                    encodedData = (unsigned char *)realloc(encodedData, encodedDataSize + extraBytes);
                    encodedDataSize += extraBytes;
                    printf("i: %d\treallocated to: %d\n",i,encodedDataSize);
                }
                encodedBytes += extraBytes;
            */

            for(j = 0; j < myHuffmanDictionary[(int)inputData[i] - 'a'].bitSequenceLength; j++){
                if(myHuffmanDictionary[(int)inputData[i] - 'a'].bitSequence[j] == '0'){
                    //bit shift by 1 to insert the 0, same as multiply the value by 2
                    bits = bits << 1;

                    //increase bits filled as we inserted a value.
                    bitsFilled++;
                }else{
                    //bit shift by 1 to insert the value.
                    //as the bits variable is made of only 0 when we shift we shift 0, so we need to do a bitwise or 01 to insert a 1. 
                    bits = (bits << 1) | 01;
                    //increase bits filled as we inserted a value.
                    bitsFilled++;
                }

                //check if we reached filled a byte.
                if(bitsFilled == 8){
                    //assign the byte value to the bits we stored.
                    encodedData[encodeIndex] = bits;

                    //reset the bits values
                    bits = 0;
                    bitsFilled = 0;

                    //increase index of bytes.
                    encodeIndex++;
                }
                /*
                    //for the situation where each value of the path is considered a char and need realloc. 
                    encodedData[encodeIndex] = myHuffmanDictionary[(int)inputData[i] - 'a'].bitSequence[j];
                    encodeIndex++;
                */
            }
            //printf("\tindex: %d\tCinput: %c\ttotal bytes: %d\n", i, inputData[i], encodeIndex);
        }
    }

    //mainly for the last process as it might have some leftover bits.
    if (bitsFilled != 0){
        for (i = 0; (unsigned char)i < 8 - bitsFilled; i++){
            bits = bits << 1;
        }
        encodedData[encodeIndex] = bits;
        encodeIndex++;
    }

    //update how many bytes have been used to encode by each process.
    encodedBlocksArray[rank] = encodeIndex;

    //stop the encodingTime timer as we are done with it.
    end = clock();
    encodingTime = (double)((end - start)) / CLOCKS_PER_SEC;


    /*
        use the gather to get how many bytes have been used to encode the text by each process and store in the array in process 0.
        as gather is blocking we are sure to get all the data we need before proceeding.
        gather+broadcast has been chosed opposed to have each process send its size to every other process to avoid excessive communication.
    */
    MPI_Gather(&encodeIndex, 1, MPI_UNSIGNED, encodedBlocksArray, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

    //broadcast from process 0 all the blocksizes so every process knows the offset it has.
    MPI_Bcast(encodedBlocksArray, numberOfProcesses, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

    //calculate the offset that each process has to account for to write on the file.
    int offset = 0;
    for(i = 0; i < rank; i++){
        offset = offset + encodedBlocksArray[i];
    }

    // open file in create(if doesn't exist beforehand) or write mode
    MPI_File_open(MPI_COMM_WORLD, argv[2], MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &mpi_compressedFile);
    
    //set the pointer to the position where the process has to write
    MPI_File_seek(mpi_compressedFile, offset, MPI_SEEK_SET);

    //write
    MPI_File_write(mpi_compressedFile, encodedData, encodedBlocksArray[rank], MPI_UNSIGNED_CHAR, &status);

    //close file
    MPI_File_close(&mpi_compressedFile);

    //stop the timer for totalTime.
    globalEnd = clock();
    totalTime = (double)((globalEnd - globalStart)) / CLOCKS_PER_SEC;
    
    //have each process print its own timings.
    printf("%d, %.3f, %.3f, %.3f\n", rank, fileProcessingTime, encodingTime, totalTime);
    
    /*
        wait for every process to be done as now we have to free the memory.
        this sync is not really required as each process has its own local data and memory allocated.
        but is added just for safety and for optional addition of shared memory.
    */
    MPI_Barrier(MPI_COMM_WORLD);

    free(inputData);
    free(encodedData);
    free(freq);
    free(encodedBlocksArray);

    MPI_Finalize();

    return 0;



}