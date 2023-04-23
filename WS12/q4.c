/**
 * Description: WS12 Question4
 * This program produces a memory leak. It runs normally and exits with code 0.
 * GDB does not appear to be useful in diagnosing the leak as it doesn't see the problem.
 * Valgrind however does identify the 4 byte leak
 *
 * Author Name: Roman Shpilberg
 * Author Emails: roman.shpilberg@sjsu.edu
 * Last Modified Date: 4/22/2023
 * Creation Date: 4/22/2023
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int* dynInt = (int*) malloc(sizeof(int));
    *dynInt = 5;
    return 0;
}

