#include <stdio.h>
#include <limits.h>

int main(void) {

   int inputNum;
   scanf("%d", &inputNum); // gets the length of the input
   
   int low=INT_MAX;
   int lowest=INT_MAX;
   int cur;
   for(int i=0; i<inputNum; i++){
      scanf("%d", &cur);
      if(cur < low){
         if(cur < lowest){
            low = lowest;
            lowest = cur;
         } else {
            low = cur;
         }
      }
   }

   printf("%d and %d\n", lowest, low);

   return 0;
}
