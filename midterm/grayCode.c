#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  unsigned int i,gi;
  int n = (argc > 1) ? atoi(argv[1]):8;
  //  char *buffer = (char *)malloc(n+1);
  for(i=0;i<1<<n;i++){
    int on, next;
    int test = n;
    gi = 0;
    for(; test>0; test--){
      on = (i>>test & 1);
      next = (i>>(test-1) & 1);
      if( gi == 0 && on ){
	gi += 1<<test;
      }
      gi +=  (on^next)<<(test-1);
    }
    printf("%d \n",gi);
  }
  return 0;
}
