#include <stdio.h>

int main(void) {
   int x;
   scanf("%d", &x);
   
   int output[32];
   
   int cursor = 0;
   //printf("Binary Conversion\n");
   while(x>0){
      output[cursor] = x%2;
      x = x/2;
      cursor++;
   }
   
   //printf("Print\n");
   for(int i=0; i<cursor; i++){
      printf("%d", output[i]);
   }
   printf("\n");

   return 0;
}
