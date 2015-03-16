//program that takes two command line arguments, a numerator and denominator
//of a non negative fraction and outputs the binary representation with the
//first portion of the repeated fractional digits printed in brackets
//(c) 2014 drsmith

#include <stdio.h>
#include <stdlib.h>

void expand(int n, int d, int *binary, int *pos){
  int digit = 1;
  //  printf("%d",*(binary+digit-1));
  int mod = n%d;  
  int bracket = 0;
  do{
    *(binary+digit-1) = n/d> 0 ? 1:0;
    if(*(pos+mod) == 0){
      *(pos+mod) = digit;
    }else{
      bracket = *(pos+mod);
      break;
    }
    digit++;
    n=mod*2;
    mod=n%d;
  }while( mod != 0 );

  int i; 
  for(i = 1; i < digit; i++){ 
    if( bracket && i == bracket-1 ){ 
      fputc('[',stdout); 
    } 
    printf("%d",*(binary+i)); 
  } 
  if(bracket){
    fputc(']',stdout); 
  } 
}
int main(int argc, char **argv)
{
  int n = atoi(argv[1]);
  int d = atoi(argv[2]);

  int *binary = malloc(d * sizeof(int));
  int *pos = calloc(d, d * sizeof(int));
  expand(n,d,binary,pos);
  return 0;
}
