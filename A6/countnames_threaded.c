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
    struct NAME_STRUCT *next;
};
typedef struct NAME_STRUCT THREAD_NAME;

#define HASHSIZE 101
static struct THREAD_NAME *hashtab[HASHSIZE]; /* pointer table */

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
THREAD_NAME *lookup(char *s){
    THREAD_NAME *np;
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
THREAD_NAME *insert(char *name){
    THREAD_NAME *np;
    unsigned hashval;
    if ((np = lookup(name)) == NULL) { /* case 1: the name is not found, create a new one */
        np = (THREAD_NAME *) malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup(name)) == NULL)
            return NULL;
        np->count = 1;
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else { np->count += 1; } /* case 2: name exists, just increment count */
    return np;
}


/*********************************************************
// function main
*********************************************************/
int main(int argc, char *argv[]){
    if(argc != 3) printf("Please enter only 2 file names");
    //TODO similar interface as A2: give as command-line arguments three filenames of numbers (the numbers in the files are newline-separated).

    printf("create first thread");
    pthread_create(&tid1,NULL,thread_runner,NULL);

    printf("create second thread");
    pthread_create(&tid2,NULL,thread_runner,NULL);

    printf("wait for first thread to exit");
    pthread_join(tid1,NULL);
    printf("first thread exited");

    printf("wait for second thread to exit");
    pthread_join(tid2,NULL);
    printf("second thread exited");

    //TODO print out the sum variable with the sum of all the numbers

    exit(0);

}//end main


/**********************************************************************
// function thread_runner runs inside each thread
**********************************************************************/
void* thread_runner(void* x)
{
    pthread_t me;

    me = pthread_self();
    printf("This is thread %ld (p=%p)",me,p);

    pthread_mutex_lock(&tlock2); // critical section starts
    if (p==NULL) {
        p = (THREADDATA*) malloc(sizeof(THREADDATA));
        p->creator=me;
    }
    pthread_mutex_unlock(&tlock2);  // critical section ends

    if (p!=NULL && p->creator==me) {
        printf("This is thread %ld and I created THREADDATA %p",me,p);
    } else {
        printf("This is thread %ld and I can access the THREADDATA %p",me,p);
    }


    /**
     * //TODO implement any thread name counting functionality you need.
     * Assign one file per thread. Hint: you can either pass each argv filename as a thread_runner argument from main.
     * Or use the logindex to index argv, since every thread will increment the logindex anyway
     * when it opens a file to print a log message (e.g. logindex could also index argv)....
     * //Make sure to use any mutex locks appropriately
     */



    // TODO use mutex to make this a start of a critical section
    if (p!=NULL && p->creator==me) {
        printf("This is thread %ld and I delete THREADDATA",me);
        /**
         * TODO Free the THREADATA object.
         * Freeing should be done by the same thread that created it.
         * See how the THREADDATA was created for an example of how this is done.
         */

    } else {
        printf("This is thread %ld and I can access the THREADDATA",me);
    }
    // TODO critical section ends

    pthread_exit(NULL);
    return NULL;

}//end thread_runner

