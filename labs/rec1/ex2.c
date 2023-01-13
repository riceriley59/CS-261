#include <stdio.h>

int function(int num, int ctr) {
  int i;
  
  //keep on dividing the number by two and adding 1 to see how many times 2 goes
  //into the number
  for(i = 2;i <= num/2; i++){
    if(num % i==0){ // if number can be divided by two
      ctr++; // increment number
      break; // break out of loop
    }
  }
  if(ctr == 0 && num != 1) // if not divisble by 2 and the number isn't one then return 1 cause prime
    return 1;
  
  else // if divisible by two return 0 and it's not a prime number
    return 0;
}

int main(){

  int num, ctr = 0, r = -1;
  printf("Input a number: ");
  scanf("%d",&num);
  
  r = function(num, ctr);
  printf("%d\n", r);

  return 0;
}
