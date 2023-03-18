#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

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
#define FILENAME_LENGTH 15
#define ARG_LENGTH 15


int main(int argc, char *argv[]) {
    char line[COLS]; // cursor for the input from stdin
    int commandIndex = 0; //  keeps track of the number of commands entered
    pid_t pid; // holds the pid for the while loop

    // reads the input commands and creates child processes
    while(fgets(&line, COLS, stdin) != NULL){
        if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = '\0'; // replace newline
        //printf("%s\n", line);


        pid = fork();
        if(pid == -1){
            printf(stderr, "Fork Failed");
            exit(1);
        } else if (pid == 0){ // child

            // Splits the line for execvp
            char* argArray[ARG_LENGTH];
            int argI = 1; // counter for the arguments
            argArray[0]=strtok(line, " ");
            while(argArray[argI] != NULL){
                argArray[argI]=strtok(NULL, " ");
                argI++;
            }

            // redirects stdout and stderr
            char fileName[FILENAME_LENGTH]; // holder for the filename of .out and .err
            snprintf(fileName, FILENAME_LENGTH, "%d.out", getpid()); // create the output file
            int fdOut = open(fileName, O_RDWR | O_CREAT | O_APPEND, 0777);
            snprintf(fileName, FILENAME_LENGTH, "%d.err", getpid()); // create the error file
            int fdErr = open(fileName, O_RDWR | O_CREAT | O_APPEND, 0777);

            // redirect stdin and stderr
            dup2(fdOut, 1);
            dup2(fdErr, 2);

            execvp(argArray[0], argArray); // executes the command

            exit(2); // exits with exit code 2 if execvp fails
        } else { // parent

        }
        commandIndex++; // increment the number of processes created
    }
    while(wait(NULL) == -1){
        continue;
    }
    return 0;
}

