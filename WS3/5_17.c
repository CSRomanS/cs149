#include <stdio.h>

int main(void) {
   const int NUM_ELEMENTS = 9;
   int userValues[NUM_ELEMENTS];    // Set of data specified by the user


   int curNum;
   int count = 0;
   do{
      scanf("%d", &curNum); 
      if(count > NUM_ELEMENTS){
         printf("Too many numbers\n");
         return 0;
      }
      
      if(curNum >= 0){
         userValues[count] = curNum;
         count++;
      }
   }while(curNum >= 0);
   
   printf("Middle item: %d\n", userValues[count/2]);
   
   return 0;
}
