/**
 * Description: This file counts the number of names in a given text file
 * Author Name: Roman Shpilberg
 * Author Emails: roman.shpilberg@sjsu.edu
 * Last Modified Date: 2/16/2023
 * Creation Date: 2/16/2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int ROWS = 100;
const int COLS = 30;

int inList(char list[ROWS][COLS], char name[COLS], int maxNames);
void printNameCount(char list[ROWS][COLS], int arrCount[ROWS], int maxNames);

int main(int argc, char *argv[]){
    char names[ROWS][COLS];
    int nameCount[ROWS];
    int nameIndex = 0; // index of the next free row

    printf("You have entered: %s\n", argv[1]);

    FILE *nameFile = fopen(argv[1], "r");

    // file error
    if(nameFile == NULL){
        printf("cannot open file\n");
        exit(1);
    }

    char nameCursor[COLS];
    int lineCount = 0; // keeps a count of the number of lines read
    while(fgets(nameCursor, COLS+1, nameFile)){
        lineCount++;
        if (nameCursor[strlen(nameCursor) - 1] == '\n') nameCursor[strlen(nameCursor) - 1] = '\0';
        if (strlen(nameCursor) == 0){ // filters empty lines
            fprintf(stderr, "\tWarning - Line %d is empty.\n", lineCount);
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
    printNameCount(names, nameCount, nameIndex);
    return 0;
}

/**
 *
 * @param list pointer to a 2d char array of names
 * @param name char[] of name to find
 * @return >0 if no name matched, the line that matched otherwise
 */
int inList(char list[ROWS][COLS], char name[COLS], int maxNames){
    for(int i=0; i<maxNames; i++){ // go through names
        //printf("Comparing [%s] to [%s]\n", list[i], name);
        for(int j=0; j<COLS; j++) { // go through specific name
            if (list[i][j] != name[j]) {
                break; // goes on to next name
            }
            //printf("\tFound Match\n");
            return i; // return the index found
        }
    }
    //printf("\tNo Match\n");
    return -1; // no match found

}

void printNameCount(char list[ROWS][COLS], int arrCount[ROWS], int maxNames){
    printf("Printing Table:\n");
    for(int i=0; i<maxNames; i++){
        printf("%s: %d\n", list[i], arrCount[i]);
    }
}