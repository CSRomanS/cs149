#include <stdio.h>

int main(void) {
   int currentPrice;
   int lastMonthsPrice;
   
   scanf("%d", &currentPrice);
   scanf("%d", &lastMonthsPrice);
   
   /* Type your code here. */
   printf("This house is $%d. The change is $%d since last month.\n", currentPrice, currentPrice-lastMonthsPrice);
   printf("The estimated monthly mortgage is $%f.\n", (currentPrice * 0.051) / 12);
   return 0;
}
