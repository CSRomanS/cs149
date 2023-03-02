#include <stdio.h>

double LapsToMiles(double numLaps){
   return numLaps*0.25;
}

int main(void) {
   double input;
   scanf("%lf", &input);
   printf("%0.2lf\n", LapsToMiles(input));

   return 0;
}
