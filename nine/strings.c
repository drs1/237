// reads from stdin and echoes strings of printable chars that are at least 4 chars long.
// (c) drsmith

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main( int argc, char **argv)
{
  int c;
  int hold[4];
  int *pt = hold;

  while( EOF != (c=fgetc(stdin))){
    if( isprint(c) ){
      //ptr arith to find where we're pointing
      int on = pt-hold;
      if( on == 3){
	*(pt++) = c;
	int i;
	for( i = 0; i<4; i++){
	fputc(*(hold+i),stdout);
	}
      }else if(on == 4){
	fputc(c,stdout);
      }else{
	*(pt++) = c;
      }
    }else{
      pt = hold;
    }
  }
  fputc('\n',stdout);
  return 0;
}
