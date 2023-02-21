#include <stdio.h>
#include <string.h>

int main(void) {
   char words[20][10];

   int inputNum;
   scanf("%d", &inputNum); // gets the length of the input
   
   for(int i=0; i<inputNum; i++){
      scanf("%s", words[i]);
   }

   char thresh;
   int match;
   scanf(" %c", &thresh);
   
   for(int i=0; i<inputNum; i++){
      match = 0;
      for(int j=0; j<10; j++){
         if(thresh == words[i][j]){
            match = 1;
            break;
         }
      }
      if(match>0){
         printf("%s,", words[i]);
      }
   }
   printf("\n");
   
   return 0;
}
