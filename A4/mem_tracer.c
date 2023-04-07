/**
 * Description:
 * Author Name: Roman Shpilberg
 * Author Emails: roman.shpilberg@sjsu.edu
 * Last Modified Date: 4/7/2023
 * Creation Date: 4/4/2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define START_ROWS 10
#define CURSOR_SIZE 200


/**
    TRACE_NODE_STRUCT is a linked list of
    pointers to function identifiers
    TRACE_TOP is the head of the list is the top of the stack
**/
struct TRACE_NODE_STRUCT {
    char* functionid;                // ptr to function identifier (a function name)
    struct TRACE_NODE_STRUCT* next;  // ptr to next frama
};

typedef struct TRACE_NODE_STRUCT TRACE_NODE;
static TRACE_NODE* TRACE_TOP = NULL;       // ptr to the top of the stack

/* --------------------------------*/
/* function PUSH_TRACE */
/*
 * The purpose of this stack is to trace the sequence of function calls,
 * just like the stack in your computer would do.
 * The "global" string denotes the start of the function call trace.
 * The char *p parameter is the name of the new function that is added to the call trace.
 * See the examples of calling PUSH_TRACE and POP_TRACE below
 * in the main, make_extend_array, add_column functions.
**/
void PUSH_TRACE(char* p)          // push p on the stack
{
    TRACE_NODE* tnode;
    static char glob[]="global";

    if (TRACE_TOP==NULL) {

        // initialize the stack with "global" identifier
        TRACE_TOP=(TRACE_NODE*) malloc(sizeof(TRACE_NODE));

        // no recovery needed if allocation failed, this is only
        // used in debugging, not in production
        if (TRACE_TOP==NULL) {
            printf("PUSH_TRACE: memory allocation error\n");
            exit(1);
        }

        TRACE_TOP->functionid = glob;
        TRACE_TOP->next=NULL;
    }//if

    // create the node for p
    tnode = (TRACE_NODE*) malloc(sizeof(TRACE_NODE));

    // no recovery needed if allocation failed, this is only
    // used in debugging, not in production
    if (tnode==NULL) {
        printf("PUSH_TRACE: memory allocation error\n");
        exit(1);
    }//if

    tnode->functionid=p;
    tnode->next = TRACE_TOP;  // insert fnode as the first in the list
    TRACE_TOP=tnode;          // point TRACE_TOP to the first node

}/*end PUSH_TRACE*/

/* --------------------------------*/
/* function POP_TRACE */
/* Pop a function call from the stack */
void POP_TRACE()    // remove the op of the stack
{
    TRACE_NODE* tnode;
    tnode = TRACE_TOP;
    TRACE_TOP = tnode->next;
    free(tnode);

}/*end POP_TRACE*/



/* ---------------------------------------------- */
/* function PRINT_TRACE prints out the sequence of function calls that are on the stack at this instance */
/* For example, it returns a string that looks like: global:funcA:funcB:funcC. */
/* Printing the function call sequence the other way around is also ok: funcC:funcB:funcA:global */
char* PRINT_TRACE()
{
    int depth = 50; //A max of 50 levels in the stack will be combined in a string for printing out.
    int i, length, j;
    TRACE_NODE* tnode;
    static char buf[100];

    if (TRACE_TOP==NULL) {     // stack not initialized yet, so we are
        strcpy(buf,"global");   // still in the `global' area
        return buf;
    }

    /* peek at the depth(50) top entries on the stack, but do not
       go over 100 chars and do not go over the bottom of the
       stack */

    sprintf(buf,"%s",TRACE_TOP->functionid);
    length = strlen(buf);                  // length of the string so far
    for(i=1, tnode=TRACE_TOP->next;
        tnode!=NULL && i < depth;
        i++,tnode=tnode->next) {
        j = strlen(tnode->functionid);             // length of what we want to add
        if (length+j+1 < 100) {              // total length is ok
            sprintf(buf+length,":%s",tnode->functionid);
            length += j+1;
        }else                                // it would be too long
            break;
    }
    return buf;
} /*end PRINT_TRACE*/


// -----------------------------------------
// function REALLOC calls realloc
// TODO REALLOC should also print info about memory usage.
// TODO For this purpose, you need to add a few lines to this function.
// For instance, example of print out:
// "File mem_tracer.c, line X, function F reallocated the memory segment at address A to a new size S"
// Information about the function F should be printed by printing the stack (use PRINT_TRACE)
void* REALLOC(void* p,int t,char* file,int line)
{

    p = realloc(p,t);
    printf("File %s, line %d, function %s reallocated the memory segment at address %p to a new size %d\n", file, line, PRINT_TRACE(), p, t);
    return p;
}

// -------------------------------------------
// function MALLOC calls malloc
// TODO MALLOC should also print info about memory usage.
// TODO For this purpose, you need to add a few lines to this function.
// For instance, example of print out:
// "File mem_tracer.c, line X, function F allocated new memory segment at address A to size S"
// Information about the function F should be printed by printing the stack (use PRINT_TRACE)
void* MALLOC(int t,char* file,int line)
{
    void* p;
    p = malloc(t);
    printf("File %s, line %d, function %s allocated new memory segment at address %p to size %d\n", file, line, PRINT_TRACE(), p, t);
    return p;
}

// ----------------------------------------------
// function FREE calls free
// TODO FREE should also print info about memory usage.
// TODO For this purpose, you need to add a few lines to this function.
// For instance, example of print out:
// "File mem_tracer.c, line X, function F deallocated the memory segment at address A"
// Information about the function F should be printed by printing the stack (use PRINT_TRACE)
void FREE(void* p,char* file,int line)
{
    printf("File %s, line %d, function %s deallocated the memory segment at address %p\n", file, line, PRINT_TRACE(), p);
    free(p);
}

#define realloc(a,b) REALLOC(a,b,__FILE__,__LINE__)
#define malloc(a) MALLOC(a,__FILE__,__LINE__)
#define free(a) FREE(a,__FILE__,__LINE__)


struct COMMAND_NODE_STRUCT{
    char* command; // pointer to a command string
    int lineIndex; // the index of the command
    struct COMMAND_NODE* next;
};
typedef struct COMMAND_NODE_STRUCT COMMAND_NODE;
static COMMAND_NODE* COMMAND_TOP = NULL;


void addCommand(char* inCommand, int inIndex){
    PUSH_TRACE("addCommand");
    COMMAND_NODE* tempNode = (COMMAND_NODE*) malloc(sizeof(COMMAND_NODE));
    tempNode->command = inCommand;
    tempNode->lineIndex = inIndex;
    if(COMMAND_TOP == NULL){
        tempNode->next = NULL;
    } else {
        tempNode->next = COMMAND_TOP;
    }
    COMMAND_TOP = tempNode;
    POP_TRACE();
}

void printCommands(){
    PUSH_TRACE("printCommands");
    COMMAND_NODE* tempNode = COMMAND_TOP; // start at the top
    while(tempNode->next != NULL){
        printf("%s->",tempNode->command);
        tempNode = tempNode->next; // go to the next node
    }
    printf("%s\n",tempNode->command); // print the final node
    POP_TRACE();
}

void freeCommandList(){
    PUSH_TRACE("freeCommandList");
    COMMAND_NODE* tempNode = COMMAND_TOP; // start at the top
    COMMAND_NODE* tempNode2; // stores the next command node while freeing the tempNode
    while(tempNode->next != NULL){
        tempNode2 = tempNode->next;
        free(tempNode);
        tempNode = tempNode2; // go to the next node
    }
    free(tempNode); // frees the final command node
    POP_TRACE();
}


void freeArray(char** commands, int commandLimit){
    PUSH_TRACE("freeArray");
    for(int i=0; i<commandLimit; i++){
        printf("Freeing[%d]:%p\n", i, commands[i]);
        free((void*) commands[i]); // frees each command line
    }
    free((void*) commands); //frees the command array
    POP_TRACE();
}

void printArray(char** commands, int numCommands){
    for(int i=0; i<numCommands; i++){
        printf("%s\n", commands[i]);
    }
}

int main()
{
    PUSH_TRACE("main");
    char inCursor[CURSOR_SIZE] = {0}; // temporarily stores the input until the proper amount of memory is allocated
    char **commands;
    int commandCount = 0;
    int commandLimit = START_ROWS;
    int longestCommand = 0; // holds the size of the longest command, used for realloc

    commands = (char**) malloc(sizeof(char*)* START_ROWS); // allocates an initial amount of command storage

    // Load up the commands from stdin
    while(fgets(&inCursor, CURSOR_SIZE, stdin) != NULL){
        if (inCursor[strlen(inCursor) - 1] == '\n') inCursor[strlen(inCursor) - 1] = '\0'; // replace newline
        if(!strcmp(inCursor, "q")) {break;}
        if(longestCommand < strlen(inCursor)) longestCommand = strlen(inCursor);
        // expands the command array if the current limit has been reached
        if(commandCount == commandLimit){
            //void* tmp = (char**) realloc(commands,sizeof(char)*(commandLimit+START_ROWS)); // expands the number of command storage space;
            void* tmp = (char**) realloc(commands,sizeof(char *) * (commandLimit+START_ROWS) + sizeof(char) * longestCommand * (commandLimit+START_ROWS)); // expands the number of command storage space;
            if (tmp == NULL){ // ends the program if realloc failed
                freeArray(commands, commandCount); // frees the dynamically allocated commands array
                fprintf(stderr, "realloc failed, exiting.");
                exit(1);
            }
            else{
                commands = tmp; // update the commands pointer if realloc was successful
                commandLimit += START_ROWS; // updates the number of allocated commands
            }
        }

        // normal loading of commands
        commands[commandCount] = (char*) malloc(sizeof(char) * (strlen(inCursor)+1)); // allocates memory for a command
        strcpy(commands[commandCount], inCursor); // copies the cursor contents to the newly allocated array
        addCommand(commands[commandCount], commandCount);
        commandCount++;

    }
    //printf("CommandCount:%d\n", commandCount);
    //printCommands();
    freeCommandList(); // frees the linked list
    freeArray(commands, commandCount); // frees the array
    POP_TRACE();
    POP_TRACE();
    return(0);
}// end main





