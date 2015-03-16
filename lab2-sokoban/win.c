// A routine for detecting a win.
// (c) 2014 danny r. smith
#include "sokoban.h"

// Birthday for advanced biometric tracking:
int BDay[] = { /* this is Duane's: */ 12, 15, 1960 };

// Check for a win (called from play, in sokoban.c).
// Return 1 iff all the BOX locations are also STORE locations.
// Duane: Note how I'm not refering directly to any part of the opaque type l!
int win(level *l)
{
  int r,c;
  int w,h;
  char cell;
  w = width(l);
  h = height(l);

  for( r=0; r < h; r++){
    for( c=0; c < w; c++){
      cell = get(l,r,c);
      if(cell & BOX){
	if(!(cell & STORE)){
	  return 0;
	}
      }
    }
  }

  for( r=0; r<h; r++){
    for( c=0; c<w; c++){
      cell=get(l,r,c);
      if(cell & BOX){
	highlight(l,r,c);
      }
    }
  }
  return 1;
}
