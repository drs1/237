// Insertion Sortie: The Battle is Joined.
// For Computer Science 237, the Great Class of Fall 2010.
// Modifications and conceptual improvements falling largely in 2011.
// With quicker sorts, still, in 2013.
// 2010-14 (c) duane a. bailey

// MAKE SURE YOU CHANGE THE LINE HIGHLIGHTED BELOW TO TEST YOUR CODE.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Functions that compare things look like:
typedef int compareFun(void*,void*);
// Here are two comparison functions
compareFun intCompare, charCompare;

// Things that sort look like this:
typedef void sorter(void*,int,int,compareFun*);

sorter bubbleSort; // old technology
sorter heapSort;   // shiny technology
sorter insertionSort; // new technology
extern sorter isort; // newest technology
sorter quickSort;  // self-referential technology
// other examples: qsort (on all unicies), and heapsort and mergesort(FreeBSD).

// CHANGE THIS LINE TO USE YOUR ASSEMBLY.  DON'T FOOL YOURSELF BY FORGETTING!
sorter *theSort = isort;

// Two different operations:
void sortNumbers(int big);  // reads, sorts, and prints numbers
                            // if big, prints 10 largest, in descending order
void sortLineCharacters(void); // reads lines and sorts the characters

// A rather simple driver:
// usage: sort [-c]
//   -c  sort characters on a line
//   otherwise, sorts numbers on input
//   -b prints only largest 10 values (conflicts with -c)
int main(int argc, char **argv)
{
  int sortLines = (argc == 2 && 0 == strcmp("-c",argv[1]));
  int big = (argc == 2 && 0 == strcmp("-b",argv[1]));
  if (sortLines) sortLineCharacters();
  else sortNumbers(big);
  return 0;
}

// swap size "size" values pointed to by herep and therep
void memswap(int size, void *herep, void *therep)
{
  char *here = (char*)herep;
  char *there = (char*)therep;
  char temp;
  while (size--) {
    temp = *here;
    *here++ = *there;
    *there++ = temp;
  }
}

// A great sort for almost sorted data.
void insertionSort(void *data, int n, int size, compareFun *c)
{
  void *left, *right;
  int nm1 = n-1;
  if (n > 1) {
    insertionSort(data,nm1,size,c); // do most of sorting
    right = ((char*)data) + nm1*size;  // full, unsigned multiply
    left = ((char*)right) - size; // next to right
    while (left >= data && c(left,right) > 0) { // short circuit
      memswap(size,left,right);
      left = ((char*)left) - size;
      right = ((char*)right) - size;
    }
  }
}

// for the academic purists
void bubbleSort(void *data, int n, int size, compareFun *c)
{
  char *p = (char*)data;
  int swapped;
  int i;
  do {
    swapped = 0; // aim lazers
    for (i = 1; i < n; i++) {
      char *right = p+(i*size);  // adjust fanny pack
      char *left = right - size; // ... a bit more
      if (c(left,right) > 0) {   // ride like the wind
	swapped++;
	memswap(size,left,right);
      }
    }
    n--; // we always seek workload reduction
  } while (swapped);
}

void pdr(void *data, int root, int children, int n, int size, compareFun *c)
{
  int big;
  void *bigp;
  void *rootp = ((char*)data)+root*size;

  while (root < children) {
    big = root*2+1;
    bigp = ((char*)data) + size*big;
    if (big+1 < n && c(bigp,bigp+size) <= 0) {
      big++;
      bigp += size;
    }
    if (c(rootp,bigp) >= 0) break;
    memswap(size,rootp,bigp);
    root = big;
    rootp = bigp;
  }
}

// heap sort: for those who thought it could never be done.
void heapSort(void *data, int n, int size, compareFun *c)
{
  // assume the data is not in heap form: build a heap. O(n) time
  int root, children = n/2;
  char *last = ((char*)data) + (n-1)*size;
  for (root = children-1; root >= 0; root--) {
    pdr(data,root,children,n,size,c);
  }
  while (n > 1) {
    memswap(size,data,last);
    n--;
    last -= size;
    pdr(data,0,n/2,n,size,c);
  }
}

int qsparti(void *data, int left, int right, int size, compareFun *c)
{
  char *p = (char*)data;
  // this may go on forever
  for (;;) { 
    while (left < right && c(p+left*size,p+right*size) < 0) right--;
    if (left >= right) return left;
    memswap(size,p+(left++)*size,p+right*size);
    while (left < right && c(p+left*size,p+right*size) < 0) left++;
    if (left >= right) return right;
    memswap(size,p+left*size,p+(right--)*size);
  }
}

void qsHelper(void *data, int left, int right, int size, compareFun *c)
{
  int pivot;
  if (left < right) { // darn. work.
    pivot = qsparti(data, left, right, size, c); // it all turns on pivot
    qsHelper(data,left,pivot-1,size,c);  // help! sort smallies
    qsHelper(data,pivot+1,right,size,c); // help! sort biggies
  }
}

// quickSort: the sort that motivates your inner big-O
void quickSort(void *data, int n, int size, compareFun *c)
{
  qsHelper(data, 0, n-1, size, c); // call in the help
}

// sort the characters that appear on a line.  line length is arbitrarily large
void sortLineCharacters()
{
  int allocated = 10;
  char *data = (char*)malloc(allocated);
  int length;
  int ch;

  for (;;) {  // read: forever (or until break)
    length = 0;
    // read next line.  EOF occurs only on a line by itself.
    for (ch = fgetc(stdin); ch != -1 && ch != '\n'; ch = fgetc(stdin)) {
      if (length == allocated) {
	allocated *= 2;
	data = (char*)realloc(data,allocated);
      }
      data[length++] = (char)ch;
    }
    // check for empty line and end of file, ie. end of input
    if (ch == -1) break;
    
    theSort(data,length,1,charCompare);
    
    printf("%.*s\n",length,data);
  }
}

// sort numbers that appear on the input.  number possibly sorted: unlimited.
void sortNumbers(int big)
{
  int allocated = 5;
  int *data = (int*)malloc(allocated*sizeof(int));
  int n = 0;
  int i;
  int value;
  
  while (1 == scanf("%d",&value)) {
    if (n == allocated) {
      allocated = 2*allocated;
      data = (int*)realloc(data,allocated*sizeof(int));
    }
    data[n++] = value;
  }

  theSort(data,n,sizeof(int),intCompare);
  if (big) {
    // A stupid, senseless waste of CPU: print 10 largest values.
    for (i = n-1; (i >= 0) && (i >= n-10); i--) {
      printf("%d\n",data[i]);
    }
  } else {
    for (i = 0; i < n; i++) {
      printf("%d\n",data[i]);
    }
  }
  free(data);
}

// how to compare integers.  pretty standard fare.
int intCompare(void *aptr, void *bptr)
{
  int *ap = (int*)aptr;
  int *bp = (int*)bptr;
  return (*ap)-(*bp);
}

// compare characters (without case folding).
int charCompare(void *aptr, void *bptr)
{
  char *ap = (char*)aptr;
  char *bp = (char*)bptr;
  char a = (*ap);
  char b = (*bp);
  return (a-b);
}

// what are you expecting here?!
