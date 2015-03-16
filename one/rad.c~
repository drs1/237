//a program that outputs the integer radical of a given number
// (c) danny r. smith

#include <stdio.h>

int main(int argc, char **argv)
{
  int last = 0;
  int n, rad, i;

  scanf("%d",&n);
  //don't want to be multiplying zeros
  rad = 1;

  //1 will always be a prime factor, start at 2
  for(i=2; i<n+1; i++){
    if( n%i == 0 ){
      if(i != last ){
	rad *= i;
      }
      //divide the factor we found out.
      n /= i;
      last = i;
      //if we found a factor, try that factor again
      i--;
    }
  }
  printf("%d",rad);
  return 0;
}
