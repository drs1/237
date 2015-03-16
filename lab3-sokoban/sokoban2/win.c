// A routine for detecting a win.  This is a straightforward implementation.
// (c) 2010-14 duane a. bailey, but freely available for academic use.
#include "sokoban.h"

// Your birthday (you should change):
int BDay[] = { 12, 15, 1960 };

// Check for a win.
// Return 1 if all the BOX locations are also STORE locations.
int win(level *l)
{
  int r,c,d;
  int result;
  int h = height(l), w = width(l);

  // check for an unstowed box (an indication of an unsolved puzzle)
  for (r = 0; r < h; r++) {
    for (c = 0; c < w; c++) {
      d = get(l,r,c);
      if ((d & BOX) && !(d & STORE)) { result = 0; goto fini; } // ie. return
    }
  }

  result = 1;

  // assertion: we have won; highlight all boxes
  for (r = 0; r < h; r++) {
    for (c = 0; c < w; c++) {
      d = get(l,r,c);
      if (d & BOX) {
        highlight(l,r,c);
      }
    }
  }

 fini:
  return result;
}
