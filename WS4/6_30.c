#include <stdio.h>

int MaxNumber(int num1, int num2, int num3, int num4){
   int max;
   if(num1 > num2) max = num1;
   else max = num2;
   
   if(num3>max) max = num3;
   if(num4>max) max = num4;
   return max;
}


int MinNumber(int num1, int num2, int num3, int num4){
   int min;
   if(num1 < num2) min = num1;
   else min = num2;
   
   if(num3<min) min = num3;
   if(num4<min) min = num4;
   return min;
}

int main(void) {

   int n1;
   int n2;
   int n3;
   int n4;
   scanf("%d %d %d %d", &n1, &n2, &n3, &n4);
   
   printf("Maximum is %d\n", MaxNumber(n1, n2, n3, n4));
   printf("Minimum is %d\n", MinNumber(n1, n2, n3, n4));

   return 0;
}
