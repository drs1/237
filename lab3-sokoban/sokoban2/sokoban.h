// Definitions important to sokoban.
// (c) 2010,2011,2013 duane a. bailey
#ifndef SOKOBAN_H
#define SOKOBAN_H
#include <curses.h>

// Dimensions of screen, as reported by curses
#define MAXROWS LINES
#define MAXCOLS COLS

/*
 * The level structure.
 * This can be improved.  In particular, it might be useful to have
 * the undo stack and statistics within this structure.
 */
typedef struct level_st {
  int levelNumber; // difficulty (0-MAXLEVEL)
  int rows, cols;  // dimensions of level
  char **pic;      // level lines (use get(l,r,c) to get elements)
  int top, left;   // margin sizes
  int worker;      // position of worker
  long int startTime; // when we began playing
} level;

// Bits representing maze locations in l->pic (or-ed together)
#define WALL   1   // there is a wall here
#define BOX    2   // there is a box here
#define STORE  4   // this is a storage location
#define WORKER 8   // worker is here
#define SPACE 16   // this is an empty space
#define HILITE 32  // this is drawn highlighted

// Location of important files:
#define SCREENLOC "screens/screen.%d"
#define HELPSCREEN "screens/HELP"
#define OMGSCREEN "screens/WORK"

// Number of different puzzle levels
// (you can start sokoban at a particular level with sokoban <levelnumber>)
#define MAXLEVEL 90

// For computing control-key values: clear the upper bits of ASCII
#define CTRL(ch) (ch&~(128|64|32))

// Encoding of direction of movement
#define NORTH 1
#define EAST 2
#define SOUTH 3
#define WEST 4


// The following value is set for positions on the stack,
// if a box must be pulled during undo.
#define PULL (1<<12)

// global variables:
extern int BDay[];     // date you were born
extern long BTime;     // when you were born, seconds since 1970
extern int SimpleWalls;// 1 = '#', 0 = graphics
extern int MaxStore;   // initial allocation for storage index array
extern int MaxUndo;    // initial allocation for undo stack (silly)
extern int *UndoStack;      // the undo stack
extern int UndoTop;    // top of the stack; points to last move
extern int MoveCount;   // number of moves (currently 1+UndoTop)

/*
 * Forward declaration of functions.
 * (The extern keyword means "if code not found here, the look outside".)
 */

// (see documentation in sokoban.c)
extern void display(level *l);
extern char get(level *l, int row, int col);
extern int go(level *l, int direction);
extern int height(level *l);
extern void help();
extern void highlight(level *l, int row, int col);
extern void initialize();
extern void message(char *msg);
extern void movePiece(level *l, int r0, int c0, int r1, int c1);
extern void mvstr(int r, int c, char *s);
extern void OMG(level *l);
extern void p2rc(int p, int *r, int *c);
extern int play(level *);
extern void pushMove(int m);
extern int rc2p(int r, int c);
extern level *readlevel(int n);
extern void shutdown();
extern int undo(level *l);
extern void update(level*l, int r, int c);
extern void updateStats(level *l);
extern int wallPic(level *l, int r, int c);
extern int width(level *l);
extern int win(level *l);
extern void work();
#endif
