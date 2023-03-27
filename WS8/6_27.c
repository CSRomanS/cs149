#include <stdio.h>
#include <string.h>

int CalcNumCharacters(char* userString, char userChar){
   int count = 0;
   for(int i=0;i<strlen(userString); i++){
      if(userString[i] == userChar){
         count++;
      }
   }
   return count;
}

int main(void) {
   char target;
   char string[30];
   scanf("%c", &target);
   scanf("%s", &string);

   int count = CalcNumCharacters(string, target);
   if(count == 1){
      printf("%d %c\n", count, target);
   } else {
      printf("%d %c's\n", count, target);
   }

   return 0;
}
