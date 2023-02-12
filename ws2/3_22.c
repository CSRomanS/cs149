#include <stdio.h>

int main(void) {

   /* Type your code here. */
   int one;
   int two;
   int three;
   int smallest;
   
   scanf("%d %d %d", &one, &two, &three);
   if(one<two){
      if(one<three){
         smallest = one;
      } else {
         smallest = three;
      }
   } else {
      if(two<three){
         smallest = two;
      } else {
         smallest = three;
      }
   }
   printf("%d\n", smallest);

   return 0;
}
