// a program to read a line of input. 
// (c) danny r. smith
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
  if(!(argc > 0 && argv[1])){
    fputs("Please enter a line number\n",stdout);
  }else{
    int find = atoi(argv[1]);
    int line = 1;
    int c;
    //thought of using fgets and working on a line level, but
    //character manipulation is more straightforward plus you don't
    //get the ugliness of using BUFSIZ 
    while(EOF != (c = fgetc(stdin))){
      if( line == find ){
	fputc(c,stdout);
      }
      //found an ascii new line char value, advance line
      if( c == '\n'){
	line++;
      }
    }
  }
  return 0;
}
