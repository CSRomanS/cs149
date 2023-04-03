#include<stdio.h>
#include <string.h>

void stripWhitespace(char* in){
   //printf("%s - Length:%d\n", in, strlen(in));
   if(in[0] == ' '){
      for(int i=0; i<strlen(in); i++){
         in[i] = in[i+1];  
      }
   }
   if(in[strlen(in)-1] == ' '){
      in[strlen(in)-1] = '\0';
   }
}

int main(void) {
   char input[100];
   printf("Enter input string:\n");
   fgets(input, 100, stdin);
   while(strcmp(input, "q\n")){
      input[strlen(input)] = '\0';
      if(strstr(input, ",") != NULL){
         char* firstName = strtok(input, ",");
         char* secondName = strtok(NULL, ",");
         stripWhitespace(firstName);
         stripWhitespace(secondName);
         printf("First word: %s\n", firstName);
         printf("Second word: %s\n", secondName);
      } else {
         printf("Error: No comma in string.\n\n");
      }
      printf("Enter input string:\n");
      //scanf("%s", input);
      fgets(input, 100, stdin);
   }

   return 0;
}