#include <stdio.h>

void SwapValues(int* userVal1, int* userVal2, int* userVal3, int* userVal4){
   int temp = *userVal1;
   *userVal1 = *userVal2;
   *userVal2 = temp;
   temp = *userVal3;
   *userVal3 = *userVal4;
   *userVal4 = temp;
}

int main(void) {
   int uV1, uV2, uV3, uV4;
   scanf("%d %d %d %d", &uV1, &uV2, &uV3, &uV4);
   SwapValues(&uV1, &uV2, &uV3, &uV4);
   printf("%d %d %d %d\n", uV1, uV2, uV3, uV4);
   return 0;
}