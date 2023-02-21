#include <stdio.h>

int main(void) {
   int numArr[20];

   int inputNum;
   scanf("%d", &inputNum); // gets the length of the input
   
   for(int i=0; i<inputNum; i++){
      scanf("%d", &numArr[i]);
   }
   
   int low;
   int high;
   scanf("%d %d", &low, &high); // gets the low and high range
   
   for(int i=0; i<inputNum; i++){
      if(numArr[i] >= low && numArr[i] <= high){
         printf("%d,", numArr[i]);  
      }
   }
   
   printf("\n");

   return 0;
}
