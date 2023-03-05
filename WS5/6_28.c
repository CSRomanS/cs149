#include <stdio.h>
#include <string.h>
#include <ctype.h>

void RemoveNonAlpha(char userString[], char userStringAlphaOnly[]){
   int i=0;
   int j=0;
   while(userString[i] != '\0'){
      if((userString[i] > 64 && userString[i] < 91) || (userString[i] > 96 && userString[i] < 123)){
         userStringAlphaOnly[j] = userString[i]; //  adds on the char if the input is within range
         j++;
      }
      i++;
   }
   userStringAlphaOnly[j] = '\0'; // terminate string
}

int main(void) {

   char input[51];
   char alphaOnly[51];
   fgets(&input, 50, stdin);
   RemoveNonAlpha(input, alphaOnly);
   printf("%s\n", alphaOnly);

   return 0;
}
