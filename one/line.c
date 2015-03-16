// a program to read a line of input. 
// (c) danny r. smith

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  int find = atoi(argv[1]);
  int line = 1;
  int c;

  //thought of using fgets and working on a line level, but
  //character manipulation is more straightforward plus you don't
  //get the ugliness of using BUFSIZ 
  while(EOF != (c = fgetc(stdin))){
    if( line == find ){
      printf("%c",c);
    }
    //found an ascii new line char value, advance line
    if( c == 10 ){
      line++;
    }
  }
  return 0;
}
