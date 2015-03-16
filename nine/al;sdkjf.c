//a program for finding a missing number integer
//(c) 2014 danny r. smith

#include <stdio.h>
#include <stdlib.h>

int cf(const void *a, const void*b);
void add(long n, long **array, long pos, int *size);
void mex(long *array, long value);

int main(int argc, char **argv)
{
  long next;
  long nitems = 0;
  int seed = 64;
  long *a = malloc(seed * sizeof(int));

  while(EOF != scanf("%ld",&next)){
    add(next, ptr_a, nitems++,&seed);
  }
  qsort(*ptr_a, nitems, sizeof(long), cf);
  mex(ptr_a, 0);
  return 1;
}

void add(long n, SOMETHING, long pos, int *size){
  if(pos > ((*size/2)-1)){
    *size = *size * 2;
    printf("mem alloced %d \n", *size);
    if( NULL == realloc(SOMETHING, *size)){
      printf("%s", "could not realloc mem \n");
    }
  }
  //  SOMETHING// **(pt2_a+pos) = n;
}
void mex(long **pt2_a, long value){
  //SOMETHING  //  long **ip = pt2_a;
  if( **ip != value ){
    printf("mex = %ld \n",value);
    return;
  }else{
    //SOMETHING    while( **ip == value ){
      //SOMETHING      *ip++;
    }
  //SOMETHING    mex(ip, ++value);
  }
}
int cf( const void *a, const void *b){
  return ( *(long*)a - *(long*)b);
}
