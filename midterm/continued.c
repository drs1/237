//program that takes two inputs, a numerator and a denominator, and prints the quotients of it's
//continued fractions
//(c) d.r.smtih

#include <stdio.h>

int main(int argc, char **argv)
{
  int top,bottom;

  scanf("%d",&top);
  scanf("%d",&bottom);

  while( bottom > 0 ){
    printf("%d ",(top/bottom));
    top -= ((top/bottom)*bottom);
    top ^= bottom;
    bottom ^= top;
    top ^= bottom;
  }
  return 0;
}
