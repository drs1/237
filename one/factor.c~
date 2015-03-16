// Find the factors of an integer
// (c) danny smith

#include <stdio.h>

int main(int argc, char **argv){
  int n,f;

  if(!scanf("%d",&n)){
    fprintf(stderr,"Don't be a twit.\n");
    return 1;
  }
  for(f = 1; f*f<=n; f++){
    if( n%f == 0 ) printf("%d\n",f);
    if( f*f != n ){
      printf("%d\n",n/f);
    }
  } 
  return 0;
}
