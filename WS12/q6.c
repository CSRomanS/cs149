/**
 * Description: WS12 Question4
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
    int i = 1;
    for(int* cur = data; cur != NULL; cur++){
        *cur = i++;
    }
    return 0;
}

