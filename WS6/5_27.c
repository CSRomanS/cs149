#include <stdio.h>
#include <string.h>

int main(void) {
   char input[51];
   fgets(input, 51, stdin);
   while(strcmp(input, "done\n") && strcmp(input, "d\n") && strcmp(input, "Done\n")){
      input[strcspn(input, "\n")] = 0;
      //printf("%s", input);
      for(int i=strlen(input)-1; i>=0; i--){
         printf("%c", input[i]);
      }
      printf("%c", '\n');
      fgets(input, 51, stdin);
   }


   return 0;
}
