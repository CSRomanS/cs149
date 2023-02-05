/**
 * Description: Lab 1.17 - Prints out a right facing arrow using two integers from input
 * Author Name: Roman Shpilberg
 * Author Email: roman.shpilberg@sjsu.edu
 * Last Modified Date: 2/4/2023
 * Creation Date: 2/4/2023
 */
#include <stdio.h>

int main() {
    int firstNum;
    int secondNum;
    scanf("%d", &firstNum);
    scanf("%d", &secondNum);

    printf("    %d\n", secondNum);
    printf("    %d%d\n", secondNum, secondNum);
    printf("%d%d%d%d%d%d%d\n", firstNum, firstNum, firstNum, firstNum, secondNum, secondNum, secondNum);
    printf("%d%d%d%d%d%d%d%d\n", firstNum, firstNum, firstNum, firstNum, secondNum, secondNum, secondNum, secondNum);
    printf("%d%d%d%d%d%d%d\n", firstNum, firstNum, firstNum, firstNum, secondNum, secondNum, secondNum);
    printf("    %d%d\n", secondNum, secondNum);
    printf("    %d\n", secondNum);

    return 0;
}


