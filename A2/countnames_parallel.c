/**
 * Description: This file counts the number of names in a number of given text file
 *              Uses a separate process for each file entered, results are piped to the parent
 * Author Name: Roman Shpilberg
 * Author Emails: roman.shpilberg@sjsu.edu
 * Last Modified Date: 3/4/2023
 * Creation Date: 3/1/2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define ROWS 100 // number of names in the array
#define COLS 31 // max length of names

int inList(char list[ROWS][COLS], char name[COLS], int maxNames); // checks array membership
void printNameCount(char list[ROWS][COLS], int arrCount[ROWS], int maxNames); // print the names and count
int loadNames(char nameTable[ROWS][COLS], int nameCount[COLS], char fileName[]); // load up name count from file

int main(int argc, char *argv[]){
    int pipes[2*(argc-1)]; // holds the pipes
    int pidArr[argc-1]; // pid to pipe correlation

    // child spawning loop
    pid_t pid;
    for(int i=0; i<argc-1; i++){
        if(pipe(&pipes[2*i])==-1){ // initialize pipe
            fprintf(stderr, "Pipe Failed");
            return 1;
        }
        pid = fork();

        if(pid == -1){
            printf(stderr, "Fork Failed");
            exit(1);
        } else if (pid == 0){
            // printf("Spawned CHILD(%d) argv[%d] - Filename:%s\n", getpid(), i+1, argv[i+1]);
            close(pipes[2*i]); // close pipe for reading

            // child's arrays for counting names
            char names[ROWS][COLS];
            int nameCount[ROWS];

            int nameIndex = loadNames(names, nameCount, argv[i+1]); // load names from the given file

            // write the list of names, the count of names, and the number of names read to the pipe
            write(pipes[2*i+1], names, sizeof(names));
            write(pipes[2*i+1], nameCount, sizeof(nameCount));
            write(pipes[2*i+1], &nameIndex, sizeof(nameIndex));
            close(pipes[2*i+1]);
            // printf("Returning from CHILD(%d)\n", getpid());
            exit(0);
        } else { // parent
            pidArr[i] = pid; // sets the relation between pipe index and child process
            close(pipes[2*i+1]); // closes parent writing end
        }
    }

    int totalNameCount = 0; // keeps a tally of all the names in the parent process

    // parent's  array for adding up the count from each child
    char names[ROWS*(argc-1)][COLS]; // size of: 100 * NumberOfFiles
    int nameCount[ROWS];

    // parent reading loop
    for(int i=0; i<argc-1; i++){
        int finishedPID = wait(NULL); // block until a child returns

        // Find the returned child's pipe
        int pipeOffset; // offset for the pipe array
        for(int j=0; j<argc-1; j++){ // iterate through the pid array to find the offset for the pipe array
            if(pidArr[j] == finishedPID){
                pipeOffset = j; // set the offset to the matched index
            }
        }
        // printf("Reading from CHILD(%d), Offset:(%d)\n", finishedPID, pipeOffset);
        // temp arrays for reading in counts from child processes
        char currentNames[ROWS][COLS];
        int currentNameCount[ROWS];
        int nameIndex; // holds the max count of names read

        // read the returned data from the pipe
        read(pipes[2*pipeOffset], currentNames, sizeof(currentNames));
        read(pipes[2*pipeOffset], currentNameCount, sizeof(currentNameCount));
        read(pipes[2*pipeOffset], &nameIndex, sizeof(nameIndex));
        close(pipes[2*pipeOffset]);
        //printf("Printing(%d)\n", pipeOffset+1);

        // adding up the names to the master namelist
        for(int j=0; j<nameIndex; j++){
            int index = inList(names, currentNames[j], totalNameCount); // get index of the name in the master name array
            if(index<0){
                strcpy(names[totalNameCount], currentNames[j]); // copy a new name into the master name array
                nameCount[totalNameCount] = currentNameCount[j]; // initialize to the returned count
                totalNameCount++;
            } else {
                nameCount[index] += currentNameCount[j]; // add up the count
            }
        }
    }
    printNameCount(names, nameCount, totalNameCount); // print out the count of names
    return 0;
}


/**
 * Loads names into a passed array
 * @param nameTable the name table to load the names into
 * @param nameCount the count of each name
 * @param fileName the filename to read
 * @return the number of names read from the file
 */
int loadNames(char nameTable[ROWS][COLS], int nameCount[COLS], char fileName[]){
    int nameIndex = 0; // index of the next free row
    FILE *nameFile = fopen(fileName, "r");
    // file error
    if(nameFile == NULL){
        printf("range: cannot open file\n");
        exit(1);
    }

    char nameCursor[COLS];
    int lineCount = 0; // keeps a count of the number of lines read from the file
    while(fgets(nameCursor, COLS+1, nameFile)){
        lineCount++;

        // strips newline characters
        if (nameCursor[strlen(nameCursor) - 1] == '\n') nameCursor[strlen(nameCursor) - 1] = '\0';

        // filters empty lines
        if (strlen(nameCursor) == 0){
            fprintf(stderr, "Warning - file %s line %d is empty.\n", fileName, lineCount);
            continue;
        }

        int index = inList(nameTable, nameCursor, nameIndex);
        if(index<0){ // if no name found, add on name
            strcpy(nameTable[nameIndex], nameCursor); // copies the name into the parent array
            nameCount[nameIndex] = 1;
            nameIndex++;
        } else{ // if name found, just increment the count
            nameCount[index] += 1; // increments the count of the found name
        }
    }
    return nameIndex;
}

/**
 * Finds if a name exists in the list
 * @param list reference to a 2d char array of names
 * @param name char[] of name to find
 * @param maxNames the number of names loaded in the list
 * @return >0 if no name matched, the line that matched otherwise
 */
int inList(char list[ROWS][COLS], char name[COLS], int maxNames){
    for(int i=0; i<maxNames; i++){ // go through names
        if(strcmp(list[i], name) == 0){
            return i;
        }
    }
    return -1; // no match found

}

/**
 * Prints out the count of names
 * @param list the list of names to print out
 * @param arrCount the count of names list
 * @param maxNames the number of names loaded in the list
 */
void printNameCount(char list[ROWS][COLS], int arrCount[ROWS], int maxNames){
    for(int i=0; i<maxNames; i++){
        printf("%s: %d\n", list[i], arrCount[i]);
    }
}