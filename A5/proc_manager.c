#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <time.h>

/**
 * Description:
 * Author Name: Roman Shpilberg
 * Author Emails: roman.shpilberg@sjsu.edu
 * Last Modified Date: 4/15/2023
 * Creation Date: 4/15/2023
 */

struct nlist { /* table entry: */
    struct nlist *next; /* next entry in chain */
    struct timespec starttime;
    struct timespec finishtime;
    int index; // this is the line index in the input text file */
    pid_t pid;  // the process id. you can use the pid result of wait to lookup in the hashtable */
    char* command; // command. This is good to store for when you decide to restart a command */
};

#define HASHSIZE 101
static struct nlist *hashtab[HASHSIZE]; /* pointer table */

/* This is the hash function: form hash value for string s */
/* You can use a simple hash function: pid % HASHSIZE */
unsigned hash(int pid){
    /* unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE; */
    return pid % HASHSIZE;
}

/* lookup: look for s in hashtab */
/* This is traversing the linked list under a slot of the hash table. The array position to look in is returned by the hash function */
struct nlist *lookup(int pid){
    struct nlist *np;
    for (np = hashtab[hash(pid)]; np != NULL; np = np->next)
        if(np->pid == pid) return np; // found
    return NULL; /* not found */
}


char* strdupMalloc(char* s);


/* insert: put (name, defn) in hashtab */
/* This insert returns a nlist node. Thus when you call insert in your main function  */
/* you will save the returned nlist node in a variable (mynode). */
/* Then you can set the starttime and finishtime from your main function: */
/* mynode->starttime = starttime; mynode->finishtime = finishtime; */
struct nlist *insert(char *command, int pid, int index){
    struct nlist *np;
    unsigned hashval;
    if ((np = lookup(pid)) == NULL) { /* case 1: the pid is not found, so you have to create it with malloc. Then you want to set the pid, command and index */
        np = (struct nlist *) malloc(sizeof(*np));
        if (np == NULL || (np->command = strdupMalloc(command)) == NULL) // sets command and checks for bad malloc
            return NULL;
        np->pid = pid;
        np->index = index;
        hashval = hash(pid);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else { } /* case 2: the pid is already there in the hashslot, i.e. lookup found the pid. In this case you can either do nothing, or you may want to set again the command  and index (depends on your implementation). */
    //free((void *) np->defn); /*free previous defn */
    return np;
}


/** You might need to duplicate the command string to ensure you don't overwrite the previous command each time a new line is read from the input file.
Or you might not need to duplicate it. It depends on your implementation. **/
char* strdupMalloc(char* s) /* make a duplicate of s */
{
    char *p;
    p = (char *) malloc(strlen(s)+1); /* +1 for ’\0’ */
    if (p != NULL)
        strcpy(p, s);
    return p;
}

#define ROWS 100 // number of commands in the array
#define COLS 31 // max length of commands
#define FILENAME_LENGTH 15
#define CURSOR_SIZE 100 // length of cursor for getting input

int splitCommands(char argArray[COLS][COLS], char inString[COLS]);
void childRun(char* fileName, char* inCursor, int commandIndex);

int main(int argc, char *argv[]) {
    char inCursor[CURSOR_SIZE] = {0}; // cursor for the input from stdin
    int commandIndex = 1; //  keeps track of the number of commands entered
    pid_t pid; // holds the pid for the while loop

    // reads the input commands and creates child processes
    int inputLength = 0; // holder for strlength of the cursor
    char fileName[FILENAME_LENGTH] = {0}; // holder for the filename of .out and .err
    while(fgets((char*) &inCursor, CURSOR_SIZE, stdin) != NULL){
        inputLength = strlen(inCursor);
        if (inCursor[inputLength - 1] == '\n') inCursor[inputLength - 1] = '\0'; // replace newline

        // Create the hash node structure for the command
        struct nlist *np = insert(inCursor, getpid(), commandIndex);
        clock_gettime(CLOCK_MONOTONIC, &np->starttime); // start timer

        pid = fork();
        if(pid == -1){
            fprintf(stderr, "Fork Failed");
            exit(1);
        } else if (pid == 0){ // child
            childRun(fileName, inCursor, commandIndex);  // runs the child process sequence
        }
        fflush(stdout);
        commandIndex++; // increment the number of processes created
    }

    int status; // holder for the status of children processes
    pid_t wpid; // returned child pid holder
    while((wpid = wait(&status)) > 0){ // wait for all children to finish
        struct nlist* reNode = lookup(wpid);
        clock_gettime(CLOCK_MONOTONIC, &reNode->finishtime); // set finish time

        //char fileName[FILENAME_LENGTH]; // holder for the filename of .out and .err
        snprintf(fileName, FILENAME_LENGTH, "%d.out", (int)wpid); // create the output file name
        FILE* childOut = fopen(fileName, "a");
        fprintf(childOut, "Finished child %d pid of parent %d\n", (int)wpid, getpid());
        fprintf(childOut, "Finished at %lld, runtime duration %lld", reNode->finishtime.tv_sec, reNode->finishtime.tv_sec-reNode->starttime.tv_sec);
        fclose(childOut);

        // parent printing to files
        snprintf(fileName, FILENAME_LENGTH, "%d.err", (int)wpid); // create the error file name
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

void childRun(char* fileName, char* inCursor, int commandIndex){
    // redirects stdout and stderr
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

    int cmds = splitCommands(commandArray, inCursor);

    for(int i=0; i<cmds; i++){
        test[i] = (char*) &commandArray[i];
    }
    test[cmds]=NULL; // null terminator for execvp

    execvp(test[0], test); // executes the command
    fprintf(stderr, "Failed to execute: %s", inCursor);
    exit(2); // exits with exit code 2 if execvp fails
}