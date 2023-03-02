#include <stdio.h>

int main(void) {
   const int NUM_ELEMENTS = 20;
   int userValues[NUM_ELEMENTS];    // Set of data specified by the user

   int inputNum;
   scanf("%d", &inputNum); // gets the length of the input
   
   for(int i=0; i<inputNum; i++){
      scanf("%d", &userValues[i]);
   }
   
   int thresh;
   scanf("%d", &thresh);
   for(int i=0; i<inputNum; i++){
      if(userValues[i] <= thresh){
         printf("%d,", userValues[i]);
      }
   }
   printf("\n");

   return 0;
}
