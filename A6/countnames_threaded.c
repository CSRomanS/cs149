/**
 * Description: This file counts the number of names in a number of given text file
 *              Uses a separate thread for each file entered
 *              Uses locks for critical sections
 *              Outputs timestamped log messages with unique log indexes
 *              Template taken from Files/code/a6
 * Author Name: Roman Shpilberg
 * Author Emails: roman.shpilberg@sjsu.edu
 * Last Modified Date: 5/8/2023
 * Creation Date: 5/1/2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define COLS 31 // max length of names

//thread mutex lock for access to the log index
pthread_mutex_t tlock1 = PTHREAD_MUTEX_INITIALIZER;


//thread mutex lock for critical sections of allocating THREADDATA
pthread_mutex_t tlock2 = PTHREAD_MUTEX_INITIALIZER;


//thread mutex lock for access to the name counts data structure
pthread_mutex_t tlock3 = PTHREAD_MUTEX_INITIALIZER;


void* thread_runner(void*);
pthread_t tid1, tid2;

//struct points to the thread that created the object.
//This is useful for you to know which is thread1. Later thread1 will also deallocate.
struct THREADDATA_STRUCT
{
    pthread_t creator;
};
typedef struct THREADDATA_STRUCT THREADDATA;

THREADDATA* p=NULL;

//variable for indexing of messages by the logging function.
int logindex=0;
int *logip = &logindex;

/**
 * Name node for use in the hash table
 */
struct NAME_STRUCT
{
    char* name;
    int count;
    pthread_mutex_t lock; // lock for this node
    struct NAME_STRUCT *next; // next node for when collision happens
};

#define HASHSIZE 101
static struct NAME_STRUCT *hashtab[HASHSIZE]; /* pointer table */

/**
 * hash from a string
 * Taken from Files/code/a5/hashtable_for_a5/dictionary_hashtable.c
 * @param s string to hash
 * @return hash value
 */
unsigned hash(char *s){
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

/**
 * Traverse the linked list to find a match
 * Modified from Files/code/a5/hashtable_for_a5/dictionary_hashtable.c
 * @param s string to match
 * @return pointer to struct if found, NULL if not found
 */
struct NAME_STRUCT *lookup(char *s){
    struct NAME_STRUCT *np;
    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return np; /* found */
    return NULL; /* not found */
}

/**
 * Inserts a new name node or increments the count
 * Modified from Files/code/a5/hashtable_for_a5/dictionary_hashtable.c
 * @param name the name to add or increment
 * @return a pointer to the node added or incremented
 */
struct NAME_STRUCT *insert(char *name){
    struct NAME_STRUCT *np;
    unsigned hashval;
    if ((np = lookup(name)) == NULL) { /* case 1: the name is not found, create a new one */
        // Creates a new node
        np = (struct NAME_STRUCT *) malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup(name)) == NULL)
            return NULL;
        np->count = 1;
        hashval = hash(name);
        np->lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;

        pthread_mutex_lock(&tlock3); // critical section starts
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
        pthread_mutex_unlock(&tlock3); // critical section starts
    } else { /* case 2: name exists, just increment count */
        pthread_mutex_lock(&np->lock); // critical section starts
        np->count += 1;
        pthread_mutex_unlock(&np->lock); // critical section starts
    }
    return np;
}

/**
 * Struct for passing filename to thread
 */
struct __myarg_t{
    char fileName[COLS];
};

/**
 * Prints and Frees the HashTable at the end of the program
 */
void printHashTable(){
    for(int i=0; i<HASHSIZE; i++){ // Traverse the hash table looking for non-empty elements
        if(hashtab[i] != NULL){
            struct NAME_STRUCT* cur = hashtab[i]; // Cursor for traversing and freeing the linked list
            while(cur != NULL){
                printf("%s: %d\n", cur->name, cur->count);
                free(cur->name); // free the name
                struct NAME_STRUCT* temp = cur;
                cur = cur->next; // go to the next node
                free(temp);
            }
        }
    }
}

/**
 * Main function, driver for the program
 * @param argc the number of arguments entered into commandline
 * @param argv the list of files entered as arguments in commandline
 * @return exit code
 */
int main(int argc, char *argv[]){
    if(argc != 3) { // runs only if two file names are entered
        printf("Please enter only 2 file names\n");
        exit(-1);
    }
    struct __myarg_t args1;
    struct __myarg_t args2;
    strcpy((char*)args1.fileName, argv[1]);
    strcpy((char*)args2.fileName, argv[2]);

    printf("============================== Log Messages ==============================\n");
    printf("create first thread\n");
    pthread_create(&tid1,NULL,thread_runner,&args1);

    printf("create second thread\n");
    pthread_create(&tid2,NULL,thread_runner,&args2);

    //printf("wait for first thread to exit");
    pthread_join(tid1,NULL);
    printf("first thread exited\n");

    //printf("wait for second thread to exit");
    pthread_join(tid2,NULL);
    printf("second thread exited\n");
    printf("============================== Name Counts ==============================\n");
    printHashTable(); // prints and frees the hash table

    exit(0);

}//end main

/**
 * Thread-safe implementation to increment and return the log index
 * @return unique log index
 */
int getLogIndex(){
    int result;
    pthread_mutex_lock(&tlock1); // critical section starts
    logindex += 1; // increments the index
    result = logindex; // sets the return
    pthread_mutex_unlock(&tlock1); // critical section starts
    return result;
}

/**
 * Prints out a message with an appended timestamp and thread/process identifiers
 * Time implementation taken from Files/code/a6/timer.c
 * @param messages the message to print in addition to the timestamp
 */
void logprint(char* messages){
    int hours, minutes, seconds, day, month, year;
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);

    hours = local->tm_hour;      	// get hours since midnight (0-23)
    minutes = local->tm_min;     	// get minutes passed after the hour (0-59)
    seconds = local->tm_sec;     	// get seconds passed after minute (0-59)

    day = local->tm_mday;        	// get day of month (1 to 31)
    month = local->tm_mon + 1;   	// get month of year (0 to 11)
    year = local->tm_year + 1900;	// get year since 1900

    // print local time
    if (hours < 12)	// before midday
        printf("Logindex %d, thread %d, PID %d, %02d/%02d/%d %02d:%02d:%02d am: %s\n", getLogIndex(), (int)pthread_self(), getpid(), month, day, year, hours, minutes, seconds, messages);
    else	// after midday
        printf("Logindex %d, thread %d, PID %d, %02d/%02d/%d %02d:%02d:%02d pm: %s\n", getLogIndex(), (int)pthread_self(), getpid(), month, day, year, hours-12, minutes, seconds, messages);

}

/**********************************************************************
// function thread_runner runs inside each thread
**********************************************************************/
void* thread_runner(void* x)
{
    pthread_t me;

    me = pthread_self();

    pthread_mutex_lock(&tlock2); // critical section starts
    if (p==NULL) {
        p = (THREADDATA*) malloc(sizeof(THREADDATA));
        p->creator=me;
    }
    pthread_mutex_unlock(&tlock2);  // critical section ends
    char *fString = malloc(sizeof(char)*100);
    if (p!=NULL && p->creator==me) {
        snprintf(fString, 100, "This is thread %ld and I created THREADDATA %p",me,p);
        logprint(fString);
    } else {
        snprintf(fString, 100,"This is thread %ld and I can access the THREADDATA %p",me,p);
        logprint(fString);
    }

    // File Reading Logic
    // Getting filename from arg x
    struct __myarg_t *m = (struct __myarg_t*) x;

    // Open file
    FILE *nameFile = fopen(m->fileName, "r");
    // file error
    if(nameFile == NULL){
        printf("Cannot open file: %s", m->fileName);
        exit(1);
    }

    snprintf(fString, 100,"opened file %s", m->fileName);
    logprint(fString);

    // Processing File
    char nameCursor[COLS]; // Cursor for getting names from a file
    int lineCount = 0; // keeps a count of the number of lines read from the file
    while(fgets(nameCursor, COLS+1, nameFile)){
        lineCount++;

        // strips newline characters
        if (nameCursor[strlen(nameCursor) - 1] == '\n') nameCursor[strlen(nameCursor) - 1] = '\0';

        // filters empty lines
        if (strlen(nameCursor) == 0){
            fprintf(stderr, "Warning - file %s line %d is empty.\n", m->fileName, lineCount);
            continue;
        }
        insert((char*)nameCursor);
    }
    fclose(nameFile); // close file


    pthread_mutex_lock(&tlock2); // critical section starts
    if (p!=NULL && p->creator==me) {
        snprintf(fString, 100, "This is thread %ld and I delete THREADDATA",me);
        logprint(fString);
        /**
         * TODO Free the THREADATA object.
         * Freeing should be done by the same thread that created it.
         * See how the THREADDATA was created for an example of how this is done.
         */
        free(p);
        p=NULL;
    } else {
        snprintf(fString, 100,"This is thread %ld and I can access the THREADDATA",me);
        logprint(fString);
    }
    pthread_mutex_unlock(&tlock2);  // critical section ends

    // deallocate the log printing string
    free(fString);
    fString = NULL;

    pthread_exit(NULL);
    return NULL;

}//end thread_runner

