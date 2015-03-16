// print the curling number of an array
// (c) drsmith
#include <stdio.h>
#include <stdlib.h>

int curl(int *a, int n);
int ncmp(int *a, int *b, int n);

int main(int argc, char **argv){
  int num;
  int n = 0;
  int size = 16;
  int *a = malloc(size*sizeof(int));
  while( EOF != scanf("%d", &num)){
    if(n>size/2){
      a = realloc(a, (size*=2)*sizeof(int));
    }
    *(a+n) = num;
    n++;
  }

  printf("%d",  curl(a,n));
  return 0;
}

int curl(int *inv, int n){
  int max = 1;
  int k = 1;
  int *pt;
  int *a = malloc(n*sizeof(int));
  int i;
  //flip the array, start from the back
  for(i = 0; i<n; i++){
    a[i] = inv[n-i-1];
  }
  //never gonna compare the same spot in the array
  //so start at 1
  for(i = 1; i<n; i++){
    pt = a+i;
    while(!(ncmp(a,pt,i))){
      k+=1;
      pt+=i;
    }
    //is it bigger than the last curling number we found?
    if(k>max){
      max = k;
    }
    k = 1;
  }
  return max;
}

int ncmp(int *a, int *b, int n){
  int i;
  for(i=0; i<n; i++){
    if(*(a+i) != *(b+i)){
      return *(a+i)-*(b+i);
    }
  }
  return 0;
}
