// A program to drive the assembly version of DrawCircle.
// Compile as usual.
// Usage: arc [r]
//    Prints a list of points in first 45 degrees of circle w/radius r.
//    r defaults to 10.

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct pair_st { int xpt,ypt; } pair;
extern int DrawCircle(int r, int i, pair display[]);

int main(int argc, char **argv)
{
  int i;
  int r;
  pair *display;
  int n;
  r = (argc>1)?atoi(argv[1]):10;
  display = (pair*)malloc(r*sizeof(pair));
  // n is actual number of points generated
  n = DrawCircle(r,0,display);
  for (i = 0; i < n; i++) {
    printf("(%d,%d)\n",display[i].xpt,display[i].ypt);
  }
  return 0;
}
