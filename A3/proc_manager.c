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
 * Last Modified Date: 3/18/2023
 * Creation Date: 3/16/2023
 */

#define ROWS 100 // number of commands in the array
#define COLS 31 // max length of commands
#define FILENAME_LENGTH 15
#define ARG_LENGTH 15

int splitCommands(char argArray[COLS][COLS], char inString[COLS]);

int main(int argc, char *argv[]) {
    char line[COLS]; // cursor for the input from stdin
    int commandIndex = 0; //  keeps track of the number of commands entered
    pid_t pid; // holds the pid for the while loop

    // reads the input commands and creates child processes
    while(fgets(&line, COLS, stdin) != NULL){
        if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = '\0'; // replace newline

        /*
        printf("%s\n", line);
        char commandArray[COLS][COLS];
        int cmds = splitCommands(commandArray, line);
        for(int i=0; i<cmds;i++){
            printf("%s\n", commandArray[i]);
        }
        */

        pid = fork();
        if(pid == -1){
            printf(stderr, "Fork Failed");
            exit(1);
        } else if (pid == 0){ // child
            // redirects stdout and stderr
            char fileName[FILENAME_LENGTH]; // holder for the filename of .out and .err
            snprintf(fileName, FILENAME_LENGTH, "%d.out", getpid()); // create the output file
            int fdOut = open(fileName, O_RDWR | O_CREAT | O_APPEND, 0666);
            snprintf(fileName, FILENAME_LENGTH, "%d.err", getpid()); // create the error file
            int fdErr = open(fileName, O_RDWR | O_CREAT | O_APPEND, 0666);
            dup2(fdOut, 1);
            close(fdOut);
            dup2(fdErr, 2);
            close(fdErr);

            fprintf(stdout,"Starting command %d: child %d pid of parent %d\n", commandIndex, getpid(), getppid());

            // Splits the line for execvp
            char commandArray[COLS][COLS]; // holds the split commands
            char* test[COLS]; // array of pointers for execvp

            int cmds = splitCommands(commandArray, line);

            for(int i=0; i<cmds; i++){
                test[i] = &commandArray[i];
            }
            test[cmds]=NULL; // null terminator for execvp


            //fprintf(stdout, "argArray:%s\n", commandArray[1]);

            execvp(test[0], test); // executes the command
            //printf("Shouldn't be here...");
            exit(2); // exits with exit code 2 if execvp fails
        }
        commandIndex++; // increment the number of processes created
    }
    while(wait(NULL) != -1){
        continue;
    }
    return 0;
}

int splitCommands(char argArray[COLS][COLS], char inString[COLS]){
    int argIndex = 0; // stores the index for a command
    int charIndex = 0; // stores the index for the argArray
    int commandCount = 1; // counts the number of commands split

    for(int i=0; i<=strlen(inString);i++) {
        if (inString[i] == ' ') { // starts writing to the next command element
            argArray[argIndex][charIndex] = '\0';
            argIndex++;
            charIndex = 0;
            commandCount++;
        } else { // copies the character
            argArray[argIndex][charIndex] = inString[i];
            charIndex++;
        }
    }
    return commandCount;
}
