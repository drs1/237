#include <stdio.h>

int main(int argc, char **argv){
  int c = 1;
  char check = (char)c;
  char *out = check != 0 ? "little" : "big";
  printf("%s\n",out);
  return 0;
}
