#include "stdio.h"


void putsit(char *s);

int main(int argc, char **argv){
  char *s = "Hello World";
  putsit(s);
  return 0;
}

void putsit( char *s){

  while( *s != '\0'){
    putchar( *(s++));
  }
  putchar('\n');
 
}
