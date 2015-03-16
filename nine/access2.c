// a program to provide all the equivalent codes given a decimal number
// (c) d.r.smith

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void deconstruct(char *code, int len);
int main(int argc, char **argv)
{
  if(!argv[1]){
    printf("Just turn the handle you twit. \n");
  }else{
    char *code = argv[1];
    int len = strlen(code);
    deconstruct(code,len);
  }
  return 0;
}
void deconstruct(char *code, int len)
{
  int subset = 0;
  int complement[10] = {9,2,1,4,3,6,5,8,7,0};
  while(subset < 1<<len){
    int *new_access = malloc(len*sizeof(int));
    int index;
    for(index = 0; index < len; index++){
      int flip = 1<<index;
      int out = *(code+index)-'0';
      if(subset & flip){
	out = *(complement+out);
      }
      *(new_access+index) = out;
    }
      int i=0;
    while(i<len){
      printf("%d",*(new_access+i));
      i++;
    }
    fputc('\n',stdout);
    subset++;
  }
}
