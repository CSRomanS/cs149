/**
 * Description: WS12 Question5
 * The program runs correctly, exiting with code 0.
 * When run with valgrind, it says the 400 byte block isn't being deallocated
 * The program runs correctly, but leaks memory
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
    data[100] = 0;
    return 0;
}

