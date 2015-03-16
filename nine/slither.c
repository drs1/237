//a program that reads in "snake" words and  deciphers whether or not the snake formation is valid
//(c) drsmith

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct points{
  int x;
  int y;
} point;

int cf( const void *a, const void *b);
void validate(point *array, int n, char* snake);
void buildSnake(char *snake, point *coords, int x, int y);

int main(int argc, char **argv)
{
  char snake[BUFSIZ];
  while(snake == fgets(snake,BUFSIZ,stdin)){
    //each hinge has a point on the end plus an extra hinge for the tail
    int n = strlen(snake)+1;
    
    point *coords = malloc(n*sizeof(point));
    point *first_crd = coords;
    point start;
    int dx;
    int dy;

    //start the snake heading up the x-axis. a snake of size zero's tail is at 
    //(0,0) and head is at (0,1);
    start.x = 0;
    start.y = 1;
    dx = start.x;
    dy = start.y;
    *coords=start;
    buildSnake(snake, first_crd, dx, dy);
    validate(coords,n,snake);
    free(coords);
  }
  return 0;
}

void buildSnake(char *snake, point *coords, int dx0, int dy0){
  char letter;

  if((letter = *snake)){
    int dx1;
    int dy1;
    if(letter == 'R'){ 
      dx1 = dy0;
      dy1 = -dx0;
    } 
    if(letter == 'L'){ 
      dx1 = -dy0;
      dy1 = dx0;
    }
    if(letter == 'S'){ 
      dx1 = dx0;
      dy1 = dy0;
    } 
    point p0 = *coords; 
    point p1; 
    p1.x = p0.x+dx1; 
    p1.y = p0.y+dy1; 
    *(coords+1) = p1; 
    buildSnake(snake+1,coords+1,dx1,dy1);
  }else{
    return;
  }
}
void validate(point *coords, int n, char *snake){
  qsort(coords, n,sizeof(*coords),cf);
  //check all the points against each other
  int i=0;
  while(i < n){
    point* test1 = coords+i++;
    point* test2 = coords+i;
    if(!(cf(test1,test2))){
      return;
    }
  }
  puts(snake);
}

int cf(const void *a, const void *b){
  point pt1 = *(point *)a;
  point pt2 = *(point *)b;
  if( !(pt1.x == pt2.x) ){
    return pt1.x - pt2.x;
  }else{
    return pt1.y - pt2.y;
  }
}
