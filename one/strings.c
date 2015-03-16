// reads from stdin and echoes strings of printable chars that are at least 4 chars long.
// (c) danny r. smith

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
      switch( pt-hold ){
      case 3:
	*pt = c;
	pt++;
	int i;
	for( i = 0; i<sizeof(hold)/sizeof(hold[0]); i++){
	  printf("%c",hold[i]);
	}
	break;
      case 4:
	printf("%c",c);
	break;
      default:
	*pt = c;
	pt++;
	break;
      }
    }else{
      pt = hold;
    }
  }
  return 0;
}

