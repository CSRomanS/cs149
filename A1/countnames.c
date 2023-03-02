/**
 * Description: This file counts the number of names in a given text file
 * Author Name: Roman Shpilberg
 * Author Emails: roman.shpilberg@sjsu.edu
 * Last Modified Date: 2/20/2023
 * Creation Date: 2/16/2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROWS 100 // number of names in the array
#define COLS 31 // max length of names

int inList(char list[ROWS][COLS], char name[COLS], int maxNames);
void printNameCount(char list[ROWS][COLS], int arrCount[ROWS], int maxNames);

int main(int argc, char *argv[]){
    char names[ROWS][COLS];
    int nameCount[ROWS];
    int nameIndex = 0; // index of the next free row

    FILE *nameFile = fopen(argv[1], "r");
    // file error
    if(nameFile == NULL){
        printf("cannot open file\n");
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
            fprintf(stderr, "Warning - Line %d is empty.\n", lineCount);
            continue;
        }

        //printf(nameCursor);
        int index = inList(names, nameCursor, nameIndex);
        if(index<0){ // if no name found, add on name
            //printf("No name Found, Adding: %s\n", nameCursor);
            for(int i=0;i<COLS;i++){
                names[nameIndex][i] = nameCursor[i]; // copy a letter

            }
            nameCount[nameIndex] = 1;
            nameIndex++;
        } else{
            nameCount[index] += 1; // increments the count of the found name
        }
    }
    printNameCount(names, nameCount, nameIndex); // print out the count of names
    fclose(nameFile);
    return 0;
}

/**
 * Finds if a name exists in the list
 * @param list pointer to a 2d char array of names
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