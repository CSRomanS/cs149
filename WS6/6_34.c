#include <stdio.h>
#include <stdbool.h>

bool IsArrayMult10(int inputVals[], int numVals){
   bool isMult = true;
   for(int i=0; i<numVals; i++){
      if(inputVals[i] % 10 != 0){
         isMult = false;
      }
   }
   return isMult;
}

bool IsArrayNoMult10(int inputVals[], int numVals){
   bool isNotMult = true;
   for(int i=0; i<numVals; i++){
      if(inputVals[i] % 10 == 0){
         isNotMult = false;
      }
   }
   return isNotMult;
}

int main(void) {
   int inArr[20];
   int count;
   scanf(" %d ", &count);
   for(int i=0; i<count;i++){
      scanf(" %d", &inArr[i]);
   }

   bool isMult = IsArrayMult10(inArr, count);
   bool isNotMult = IsArrayNoMult10(inArr, count);
   
   if(!isMult && !isNotMult){
      printf("mixed values\n");
   }
   if(isMult){
      printf("all multiples of 10\n");
   } else if(isNotMult){
      printf("no multiples of 10\n");
   } 

   return 0;
}
