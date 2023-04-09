#ifndef ITEM_TO_PURCHASE_H
#define ITEM_TO_PURCHASE_H

struct ItemToPurchase{
    int itemPrice;
    int itemQuantity;
    char itemName[];
};

void MakeItemBlank();
void PrintItemCost();

#endif