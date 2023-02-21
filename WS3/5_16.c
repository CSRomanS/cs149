#include <stdio.h>

int main(void) {
   const int NUM_ELEMENTS = 20;         // Number of input integers
   int userVals[NUM_ELEMENTS];          // Array to hold the user's input integers

   int numCursor;
   int count = 0;
   while((scanf("%d", &numCursor)) != EOF){
      userVals[count] = numCursor; // adds on the number to the list
      count++;
   }
   
   for(int i=count-1; i>0; i--){
      printf("%d,", userVals[i]);
   }
   printf("\n");

   return 0;
}
