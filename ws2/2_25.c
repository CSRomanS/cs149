#include <stdio.h>

int main(void) {

   int num;
   int denom;

   scanf("%d %d", &num, &denom);

   int first = num/denom;
   int second = first/denom;
   int third = second/denom;

   printf("%d %d %d\n", first, second, third);

   return 0;
}
