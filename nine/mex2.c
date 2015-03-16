//a program for finding a missing number integer
//(c) 2014 danny r. smith
#include <stdlib.h>
#include <stdio.h>

int cf(const void *a, const void*b);
int main(int argc, char **argv)
{
  long next;
  long pos = 0;
  long size = 64;  
  long *a = malloc(size * sizeof(long));
  while(EOF != scanf("%ld",&next)){
    if( pos > ((size/2)-1)){
      size <<= 1;
      printf("realloced mem");
      if( NULL == (long *)realloc(a, size)){
	printf("%s", "could not realloc mem \n");
      }
    }
    *(a+pos++)=next;
  }
  return 0;
}


