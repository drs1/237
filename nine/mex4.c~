//a program that returns the minimum excluded integer in a list.
// (c) d.r.smith

#include <stdio.h>
#include <stdlib.h>

int cf(const void *a, const void*b);
void mex(int n, long *array);
int main(int argc, char **argv)
{
  int num;
  int size=64;
  long *array = malloc(size*sizeof(int));
  long pos = 0;
  //read in
  while( EOF != (scanf("%d",&num))){
    if(pos > (size/2)){
      array = realloc(array, (size*=2)*sizeof(int));
    }
    *(array+(pos++)) = num;
  }
  //sort
  mex(pos,array);
  return 0;
}
void mex(int n, long *array){
  qsort(array, n, sizeof(long), cf);
  int check = 0;
  int next = 1;
  while(next < n){
    if(! (*(array+check) == *(array+next) || (*(array+check)+1 == *(array+next)))){
      printf("mex -> %ld \n",*(array+check)+1);
      return;
    }
    check = next++;
  }
  printf("there is no minimum excluded integer for the list provided");
  return ;
}
int cf( const void *a, const void *b){
  return ( *(long*)a - *(long*)b);
}
