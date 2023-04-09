#include<stdio.h>
#include<string.h>

#include "ItemToPurchase.h"

void MakeItemBlank(struct ItemToPurchase* item){
    item->itemName[0] = 'n';
    item->itemName[0] = 'o';
    item->itemName[0] = 'n';
    item->itemName[0] = 'e';
    item->itemName[0] = '\0';
    item->itemPrice = 0;
    item->itemQuantity = 0;
}

void PrintItemCost(struct ItemToPurchase* item){
    printf("%s %d @ $%d = $%d\n", item->itemName, item->itemQuantity, item->itemPrice, item->itemPrice * item->itemQuantity);
}