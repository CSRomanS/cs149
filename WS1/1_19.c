/**
 * Description: Lab 1.19 - Prints out the square and cube of a number, also the addition and multiplication of another
 * Author Name: Roman Shpilberg
 * Author Email: roman.shpilberg@sjsu.edu
 * Last Modified Date: 2/4/2023
 * Creation Date: 2/4/2023
 */
#include <stdio.h>

int main() {
    int firstInt;
    int secondInt;

    printf("Enter integer:\n");
    scanf("%d", &firstInt);
    printf("You entered: %d\n", firstInt);
    printf("%d squared is %d\n", firstInt, firstInt*firstInt);
    printf("And %d cubed is %d!!\n", firstInt, firstInt*firstInt*firstInt);

    printf("Enter another integer:\n");
    scanf("%d", &secondInt);
    printf("%d + %d is %d\n", firstInt, secondInt, firstInt+secondInt);
    printf("%d * %d is %d\n", firstInt, secondInt, firstInt*secondInt);
    return 0;
}


