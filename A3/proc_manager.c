#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

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

int splitCommands(char argArray[COLS][COLS], char inString[COLS]);

int main(int argc, char *argv[]) {
    char line[COLS]; // cursor for the input from stdin
    int commandIndex = 1; //  keeps track of the number of commands entered
    pid_t pid; // holds the pid for the while loop

    // reads the input commands and creates child processes
    while(fgets(&line, COLS, stdin) != NULL){
        if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = '\0'; // replace newline

        pid = fork();
        if(pid == -1){
            fprintf(stderr, "Fork Failed");
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
            fflush(stdout);
            // Splits the line for execvp
            char commandArray[COLS][COLS]; // holds the split commands
            char* test[COLS]; // array of pointers for execvp

            int cmds = splitCommands(commandArray, line);

            for(int i=0; i<cmds; i++){
                test[i] = &commandArray[i];
            }
            test[cmds]=NULL; // null terminator for execvp


            execvp(test[0], test); // executes the command
            fprintf(stderr, "Failed to execute: %s", line);
            exit(2); // exits with exit code 2 if execvp fails
        }
        fflush(stdout);
        commandIndex++; // increment the number of processes created
    }

    int status; // holder for the status of children processes
    pid_t wpid; // returned child pid holder
    while((wpid = wait(&status)) > 0){ // wait for all children to finish
        char fileName[FILENAME_LENGTH]; // holder for the filename of .out and .err
        snprintf(fileName, FILENAME_LENGTH, "%d.out", wpid); // create the output file name
        FILE* childOut = fopen(fileName, "a");
        fprintf(childOut, "Finished child %d pid of parent %d", wpid, getpid());
        fclose(childOut);

        // parent printing to files
        snprintf(fileName, FILENAME_LENGTH, "%d.err", wpid); // create the error file name
        FILE* childErr = fopen(fileName, "a");
        if(WIFSIGNALED(status)){
            fprintf(childErr, "Killed with signal %d", WTERMSIG(status)); // prints if the child was killed by signal
        } else {
            fprintf(childErr, "Exited with exitcode = %d", status); // prints the exitcode of the finished child
        }
        fclose(childErr);
    }
    return 0;
}

/**
 * Splits a line by spaces
 * @param argArray The array to load commands into
 * @param inString The string to split
 * @return the number of args loaded into the array
 */
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