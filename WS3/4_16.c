#include <stdio.h>

int main(void) {
   int count = 0;
   int curNum = 0;
   int max = -1;
   float sum = 0;
   do{
      scanf("%d", &curNum); 
      if(curNum >= 0){
         sum += curNum; // adds onto the sum
         count++;
      }
      if(curNum > max) max = curNum;
   }while(curNum >= 0);
   
   printf("%d %0.2lf\n", max, sum/count);
   
   return 0;
}
