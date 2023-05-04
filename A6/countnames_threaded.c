/**
 * Description: This file counts the number of names in a number of given text file
 *              Uses a separate thread for each file entered
 * Author Name: Roman Shpilberg
 * Author Emails: roman.shpilberg@sjsu.edu
 * Last Modified Date: 5/1/2023
 * Creation Date: 5/1/2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define ROWS 100 // number of names in the array
#define COLS 31 // max length of names

//thread mutex lock for access to the log index
//TODO you need to use this mutexlock for mutual exclusion
//when you print log messages from each thread
pthread_mutex_t tlock1 = PTHREAD_MUTEX_INITIALIZER;


//thread mutex lock for critical sections of allocating THREADDATA
//TODO you need to use this mutexlock for mutual exclusion
pthread_mutex_t tlock2 = PTHREAD_MUTEX_INITIALIZER;


//thread mutex lock for access to the name counts data structure
//TODO you need to use this mutexlock for mutual exclusion
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

//The name counts.
// You can use any data structure you like, here are 2 proposals: a linked list OR an array (up to 100 names).
//The linked list will be faster since you only need to lock one node, while for the array you need to lock the whole array.
//You can use a linked list template from A5. You should also consider using a hash table, like in A5 (even faster).
struct NAME_STRUCT
{
    char* name;
    int count;
    pthread_mutex_t lock;
    struct NAME_STRUCT *next;
};
//typedef struct NAME_STRUCT THREAD_NAME;

#define HASHSIZE 101
static struct NAME_STRUCT *hashtab[HASHSIZE]; /* pointer table */

/**
 * hash from a string
 * Gotten from Files/code/a5/hashtable_for_a5/dictionary_hashtable.c
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
 * Gotten from Files/code/a5/hashtable_for_a5/dictionary_hashtable.c
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
 * Gotten from Files/code/a5/hashtable_for_a5/dictionary_hashtable.c
 * @param name
 * @return
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

void printHashTable(){
    for(int i=0; i<HASHSIZE; i++){
        if(hashtab[i] != NULL){
            struct NAME_STRUCT* cur = hashtab[i];
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

/*********************************************************
// function main
*********************************************************/
int main(int argc, char *argv[]){
    if(argc != 3) {
        printf("Please enter only 2 file names\n");
        return 0;
    }
    struct __myarg_t args1;
    struct __myarg_t args2;
    strcpy((char*)args1.fileName, argv[1]);
    strcpy((char*)args2.fileName, argv[2]);

    printf("============================== Log Messages ==============================\n");
    //TODO similar interface as A2: give as command-line arguments three filenames of numbers (the numbers in the files are newline-separated).
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

    //TODO print out the sum variable with the sum of all the numbers
    printf("============================== Name Counts ==============================\n");
    printHashTable();

    exit(0);

}//end main

int getLogIndex(){
    int result;
    pthread_mutex_lock(&tlock1); // critical section starts
    logindex += 1; // increments the index
    result = logindex; // sets the return
    pthread_mutex_unlock(&tlock1); // critical section starts
    return result;
}

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
    char nameCursor[COLS];
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
    /**
     * //TODO implement any thread name counting functionality you need.
     * Assign one file per thread. Hint: you can either pass each argv filename as a thread_runner argument from main.
     * Or use the logindex to index argv, since every thread will increment the logindex anyway
     * when it opens a file to print a log message (e.g. logindex could also index argv)....
     * //Make sure to use any mutex locks appropriately
     */



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

