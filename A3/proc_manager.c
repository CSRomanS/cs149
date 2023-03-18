#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Description: This file accepts arguments that are executed in parallel
 * Logs and Errors are logged into files named after pid
 * Author Name: Roman Shpilberg
 * Author Emails: roman.shpilberg@sjsu.edu
 * Last Modified Date: 3/17/2023
 * Creation Date: 3/16/2023
 */

#define ROWS 100 // number of commands in the array
#define COLS 31 // max length of commands


int main(int argc, char *argv[]) {
    char line[COLS];
    // reads the input commands
    while(fgets(&line, COLS, stdin) != NULL){
        if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = '\0'; // replace newline
        printf("%s\n", line);

    }
    return 0;
}

