#include<stdio.h>
#include<string.h>
#include <malloc.h>

#include "ItemToPurchase.h"

int main(void) {
    struct ItemToPurchase* item1;
    char inStr[100];

    printf("Item 1\nEnter the item name:\n");
    //scanf("%s\n", &inStr);
    fgets((char*)&inStr, 100, stdin);
    if (inStr[strlen(inStr) - 1] == '\n') inStr[strlen(inStr) - 1] = '\0';
    item1 = (struct ItemToPurchase*) malloc( sizeof(item1) + sizeof(char)*strlen(inStr));
    strcpy(item1->itemName, inStr);
    printf("Enter the item price:\n");

    scanf("%d", &item1->itemPrice);
    printf("Enter the item quantity:\n");
    scanf("%d", &item1->itemQuantity);
    printf("\n");


    char c = getchar();
    while(c != '\n' && c != EOF) {
        c = getchar();
    }


    struct ItemToPurchase* item2;

    printf("Item 2\nEnter the item name:\n");
    fgets((char*)&inStr, 100, stdin);
    if (inStr[strlen(inStr) - 1] == '\n') inStr[strlen(inStr) - 1] = '\0';
    item2 = (struct ItemToPurchase*) malloc( sizeof(item2) + sizeof(char [strlen(inStr)+1]));
    strcpy(item2->itemName, inStr);
    printf("Enter the item price:\n");
    scanf("%d", &item2->itemPrice);
    printf("Enter the item quantity:\n");
    scanf("%d", &item2->itemQuantity);
    printf("\n");

    printf("TOTAL COST\n");
    PrintItemCost(item1);
    PrintItemCost(item2);
    printf("\nTotal: $%d\n", item1->itemQuantity*item1->itemPrice+item2->itemQuantity*item2->itemPrice);

    return 0;
}