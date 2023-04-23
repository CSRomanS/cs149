/**
 * Description: WS12 Question6
 * This program runs to completion normally, exiting with code 0
 * Valgrind however, does complain about an "Invalid read" on a freed memory segment
 *  It does report that there are no memory leaks
 *
 * Author Name: Roman Shpilberg
 * Author Emails: roman.shpilberg@sjsu.edu
 * Last Modified Date: 4/22/2023
 * Creation Date: 4/22/2023
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int* data = (int*) malloc(sizeof(int)*100);
    data[50] = 10;
    free(data);
    printf("%d", data[50]);
    return 0;
}

