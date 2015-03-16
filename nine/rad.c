//a program that outputs the integer radical of a given number
// (c) drsmith

#include <stdio.h>

int rad(n){
  int rad,factor;
  if(n<2){
    return -1;
  }
  rad = 1;
  factor = 2;
  while(n > 1){
    if( n%factor == 0 ){
      rad *= factor;
      do{
	n/=factor;
      }while( n%factor == 0);
    }
    factor++;
  }
  printf("%d\n",rad);
  return rad;
}

int main(int argc, char **argv)
{
  int n;
  while(scanf("%d",&n) == 1){
    rad(n);
  }
  return 0;
}
