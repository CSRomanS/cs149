#include <stdio.h>
#include <string.h>

int main(void) {
   
   char inString[51];
   int count = 0;
   
   fgets(&inString, 51, stdin);
   int i = 0;
   while(inString[i] != '\0'){
      if(inString[i] != ' ' && inString[i] != '.' && inString[i] != '!' && inString[i] != ','){
         count++;
      }
      i++;
   }
   printf("%d\n", count);
   return 0;
}
