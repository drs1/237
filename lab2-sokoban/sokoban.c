/*
 * A curses-based implementation of Sokoban, the classic Japanese puzzle.
 * (c) 2010, 2011, 2013, 2014 duane a. bailey
 *
 * See screens/HELP, or type '?' in the running game for help on the puzzle.
 * See man ncurses for help understanding the curses interface.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include "sokoban.h"

//
// Global variables.
//
int SimpleWalls = 0; // 0 = graphics walls, 1 = '#'-style walls
int MaxStore = 10;   // initial allocation for storage index array
int MaxUndo = 10;    // initial allocation for undo stack (silly)
int *UndoStack;      // the undo stack
int UndoTop = -1;    // top of the stack; points to last move
int MoveCount = 0;   // number of moves (currently 1+UndoTop)
long BTime;          // when you were born

/****************************************************************************
 * Code
 */
// initialize the screen; called once.
void initialize()
{
  struct tm bd[1];
  // curses initialization:
  initscr();
  cbreak();
  noecho();
  nonl();
  intrflush(stdscr, FALSE);
  keypad(stdscr,TRUE);
  // initial undo stack allocation
  UndoStack = (int*)malloc(MaxUndo*sizeof(int));

  // birthday determination for biometrics
  bd->tm_sec = bd->tm_min = bd->tm_hour = 0;
  bd->tm_mday = BDay[1];
  bd->tm_mon = BDay[0]-1;
  bd->tm_year = BDay[2]-1900;
  BTime = mktime(bd);

  message("Welcome to Sokoban -- type '?' for help, '^G' to quit.");
}

// Read level n from a screen file (0 to 90).
// This program starts at level 1, with 0 for experimentation.
// Select the initial level at the command line; e.g. sokoban 0
level *readLevel(int n)
{
  FILE *lf;
  char levelName[80];
  level *result;
  char buffer[80];
  int r, c;

  // attempt to open the level
  sprintf(levelName,SCREENLOC,n);
  lf = fopen(levelName,"r");
  if (lf == 0) {
    shutdown();  // we need to reset the terminal, cleanly
    fprintf(stderr,"Could not open file %s\n",levelName);
    exit(1);
  }

  // Ok, we're good to allocate structures and read level
  result = (level *)malloc(sizeof(level));
  assert(result);
  result->levelNumber = n;
  result->rows = 0;
  result->cols = 0;
  result->pic = (char**)malloc(MAXROWS*sizeof(char*));
  result->worker = 0;

  // read in the rows
  while (fgets(buffer,80,lf)) {
    int l = strlen(buffer);
    // remove final newline, if it's there
    if (l && buffer[l-1] == '\n') {
      buffer[l-1] = '\0';
      l--;
    }
    // copy into picture array
    result->pic[result->rows] = strdup(buffer);
    result->rows++;
    if (l > result->cols) result->cols = l;
  }

  // make it just the right size
  result->pic = (char **)realloc(result->pic, result->rows*sizeof(char*));

  // we now scan across the picture and find worker and boxes
  // we convert the representation to a bit-based format

  // STORE is set if this is a possible box destination (.)
  // BOX is set if there is a box ($) here
  // WORKER is set if the worker is standing here (@ or +)
  // SPACE is set if this is a possible location for the worker
  // WALL is set if this is a wall (#); nothing can go here
  for (r = 0; r < result->rows; r++) {
    for (c = 0; c < result->cols; c++) {
      int p = rc2p(r,c);
      char ch = result->pic[r][c];
      if (ch == '@') {
	result->worker = p;
	ch = WORKER;
      } else if (ch == '.' || ch == '*' || ch == '+') {
	if (ch == '.') ch = STORE | SPACE;
	else if (ch == '*') ch = STORE | BOX;
	else if (ch == '+') {
	  result->worker = p;
	  ch = STORE | WORKER;
	}
      } else if (ch == '$') {
	ch = BOX;
      } else if (ch == ' ') {
	ch = SPACE;
      } else if (ch == '#') {
	ch = WALL;
      }
      result->pic[r][c] = ch;
    }
  }

  // record the time we start this level
  result->startTime = time(0);

  // you will note that MoveCount and UndoTop are related; they needn't be
  // reset the stack top
  UndoTop = -1;
  // reset the move counter
  MoveCount = 0;
  return result;
}

// display the current level in its current state
void display(level*l)
{
  int r, c, i;

  // set these for later use: these margins are computed
  l->top = (MAXROWS-l->rows)/2;
  l->left = (MAXCOLS-l->cols)/2;

  // clear the screen (faster way? see man ncurses)
  for (i = 1; i < MAXROWS; i++) {
    move(i,0); clrtoeol();
  }

  // draw the level
  for (r = 0; r < l->rows; r++) {
    char *line = l->pic[r];
    // we're always worried: the lines are variable length
    int lineLen = strlen(line);
    for (c = 0; c < lineLen; c++) {
      // update is responsible for determining the correct representation
      update(l,r,c);
    }
  }
  // update biometric stats
  updateStats(l);
  // repaint screen
  refresh();
}

// go through the motions of play
// read in a key and act on it
// most controls follow the emacs movement keys.  we also support ^U for
// multiplying the power of commands
int play(level *l)
{
  int done = 0;        // true when finished
  int repeatCount = 0; // number of outstanding times to repeat key
  int prefix = 0;      // true if the key can't be repeated

  while (!done) {
    prefix = 0;
    int ch = getch();
    do {
      switch (ch) {
	// basic motion: take emacs or, god forbid, arrow keys
      case CTRL('B'): if (!go(l,WEST)) repeatCount = 0; break;
      case CTRL('F'): if (!go(l,EAST)) repeatCount = 0; break;
      case CTRL('N'): if (!go(l,SOUTH)) repeatCount = 0; break;
      case CTRL('P'): if (!go(l,NORTH)) repeatCount = 0; break;

	// undo last move
      case CTRL('_'): if (!undo(l)) repeatCount = 0; break;

	// the boss key (not repeatable)
      case ' ': OMG(l); repeatCount = 0; break;

	// the help key (not repeatable)
      case '?': help(l); repeatCount = 0; break;

	// the repeat key (sets the repeat count to 4, or 4*repeat count)
      case CTRL('U'):
	if (repeatCount < 1) repeatCount = 4;
	else repeatCount *= 4;
	prefix = 1;
	break;

	// loser key: quit puzzle
      case CTRL('G'):
	  shutdown();
	  exit(0);

	// otherwise, silently do nothing
      default:
	break;
      }

      // check for win; if a win, indicate message, read a key, end play
      if (win(l)) {
	updateStats(l);
	message("YOU WIN! (Press 'g' for next level.)");
	refresh();
	while ('g' != getch());
	repeatCount = 0;
	done = 1;
      } else {
	updateStats(l);
	refresh();
	if (!prefix) if (repeatCount) repeatCount--;
      }
    } while (repeatCount && !prefix);
  }
  // end of level: clear the screen
  clear();
  return 1;
}

// move worker in the indicated direction
int go(level *l, int direction)
{
  // To move we need to:
  //  1. Verify that there is a space in the direction we're headed, or
  //  2. There is BOX in that location, and a space in the direction beyond.
  // If case 1, we simply move there, leaving a space behind.
  // If case 2, we move the BOX to the space, the worker to the BOX location
  //   and leave a space behind.
  int r, c; // row and column of worker
  int dr = 0, dc = 0; // delta row, col
  int sr, sc; // space row, col
  int gr, gc; // box row, col
  int sch;
  int moved = 0; // return true if the worker was moved
  int p;

  // first, convert direction to a change in row and column:
  switch (direction) {
    case NORTH: dr = -1; break;
    case EAST: dc = 1; break;
    case SOUTH: dr = 1; break;
    case WEST: dc = -1; break;
  }
  // get worker location
  p2rc(l->worker,&r,&c);
  // compute hoped-for space location
  sr = r+dr; sc = c+dc;
  // check for space
  sch = get(l,sr,sc);
  if (sch & SPACE) {
    // we can move the worker from (r,c) to (sr,sc)
    movePiece(l,r,c,sr,sc);    
    // record the move on the undo stack
    p = rc2p(r,c);
    pushMove(p);
    // record the fact we moved
    moved = 1;
  } else if (sch & BOX) {
    // if not a space, hope it must be a box, to be pushed
    // this is the box location:
    gr = sr; gc = sc;
    // this is the new space location (behind box)
    sr = gr+dr; sc = gc+dc;
    // get the symbol at the location:
    sch = get(l,sr,sc);
    if (sch & SPACE) {
      // all good: move box to space, worker to former box location
      movePiece(l,gr,gc,sr,sc);
      movePiece(l,r,c,gr,gc);
      // record a push-style move
      p = rc2p(r,c);
      pushMove(p | PULL);
      // we moved
      moved = 1;
    }
  }
  if (moved) MoveCount++;
  return moved;
}

// curses rundown resets terminal to function normally
// must be called on any exit
void shutdown()
{
  endwin();
}

/**************************************************************************
 * Utility methods
 */
// convert row and column to a position
int rc2p(int r, int c)
{
  return (r<<6)|c;
}

// convert a position to a row and column
void p2rc(int p, int *r, int *c)
{
  *r = (p >> 6) & 0x3f;
  *c = p & 0x3f;
}

// return the width of the level
int width(level *l)
{
  return l->cols;
}

// return the height of the level
int height(level *l)
{
  return l->rows;
}

// get the descriptor of the cell at [row,col] in level l
char get(level *l, int row, int col)
{
  int len;
  char result;

  // if to left, above, or below maze, return a space
  if (row < 0 || col < 0 || row >= l->rows) return SPACE;

  // if to the right, return a space
  len = strlen(l->pic[row]);
  if (col >= len) return SPACE;

  // otherwise return the value at the desired location
  result = l->pic[row][col];
  return result;
}

// highlight the object at [row,col]
void highlight(level *l, int row, int col)
{
  int len;

  // if to left, above, or below maze, return
  if (row < 0 || col < 0 || row >= l->rows) return;

  // if to the right, return
  len = strlen(l->pic[row]);
  if (col >= len) return;

  // otherwise invert the desired location
  l->pic[row][col] |= HILITE;
  update(l,row,col);
}

// Determine the symbol needed to represent this portion of wall
int wallPic(level *l, int r, int c)
{
  // we walk around from North (1), to East (2), South (4), and West (8) and we
  // keep track of all the surrounding walls.  From this, we pick the
  // different symbols that determine the wall character here, at [r,c]
  int pattern = 0;
  /* (south = ' ', west = ' '):
   *            north = ' ', north = '#'
   * east = ' '     PLUS      VLINE
   * east = '#'     HLINE     LLCORNER
   * (south = '#', west = ' '):
   *            north = ' ', north = '#'
   * east = ' '     VLINE     VLINE
   * east = '#'     ULCORNER  LTEE
   * (south = ' ', west = '#'):
   *            north = ' ', north = '#'
   * east = ' '     HLINE     LRCORNER 
   * east = '#'     HLINE     BTEE
   * (south = '#', west = '#'):
   *            north = ' ', north = '#'
   * east = ' '     URCORNER  RTEE
   * east = '#'     TTEE      PLUS
   */
  // see ncurses.h for these symbol values
  int border[] = { ACS_PLUS, ACS_VLINE, ACS_HLINE, ACS_LLCORNER,
                   ACS_VLINE, ACS_VLINE, ACS_ULCORNER, ACS_LTEE,
		   ACS_HLINE, ACS_LRCORNER, ACS_HLINE, ACS_BTEE,
		   ACS_URCORNER, ACS_RTEE, ACS_TTEE, ACS_PLUS};
  if (SimpleWalls) return '#';
  else {
    if (WALL & get(l,r-1,c)) pattern |= 1;
    if (WALL & get(l,r,c+1)) pattern |= 2;
    if (WALL & get(l,r+1,c)) pattern |= 4;
    if (WALL & get(l,r,c-1)) pattern |= 8;
    return border[pattern];
  }
}

// update (possible) changes to what appears on the screen at [r,c]
void update(level*l, int r, int c)
{
  int ch = l->pic[r][c];
  int pleaseHighlight = 0;

  // order of these tests is important
  if (HILITE & ch) pleaseHighlight = 1;
  if (WALL & ch) ch = wallPic(l,r,c);
  else if (((WORKER | STORE) & ch) == (WORKER|STORE)) ch = '+';
  else if (((BOX | STORE) & ch) == (BOX|STORE)) ch = '*';
  else if (WORKER & ch) ch = '@';
  else if (BOX & ch) ch = '$';
  else if (STORE & ch) ch = '.';
  else if (SPACE & ch) ch = ' ';
  else assert(0); // should never make it here
  // curses move and add character to screen
  if (pleaseHighlight) ch |= A_REVERSE;
  mvaddch(r+l->top,c+l->left,ch);
}

// move a piece at [r0,c0] to empty space [r1,c1]
void movePiece(level *l, int r0, int c0, int r1, int c1)
{
  int r, c;
  int ch0,ch1;
  // get worker location to see if worker is getting moved
  p2rc(l->worker,&r,&c);
  if (r0 == r && c0 == c) { l->worker = rc2p(r1,c1); } // if so, update pos
  ch0 = l->pic[r0][c0]; // character at source
  ch1 = l->pic[r1][c1]; // character at destination
  // clear box and worker bits at source
  l->pic[r0][c0] = SPACE | (ch0 & ~(BOX|WORKER));
  // copy them to the destination
  l->pic[r1][c1] = (ch1 & ~(SPACE|BOX|WORKER)) | (ch0 & (BOX | WORKER));
  // repaint both
  update(l,r0,c0);
  update(l,r1,c1);
}

// push move onto the undo stack, possibly extending it
void pushMove(int m)
{
  UndoTop = UndoTop+1;
  if (UndoTop >= MaxUndo) {
    MaxUndo *= 2;
    UndoStack = (int*)realloc(UndoStack,MaxUndo*sizeof(int));
  }
  // store move on stack top
  UndoStack[UndoTop] = m;
}

// back up one move, possibly pulling a box
int undo(level *l)
{
  int p;
  int pull;
  if (UndoTop < 0) return 0;
  
  // grab last move
  p = UndoStack[UndoTop--];
  // is it a pull?
  pull = PULL & p;
  
  // move the worker to location p
  // PULL is set if we need to pull a box
  int sr,sc;
  int r,c;
  // move worker into space
  p2rc(l->worker, &r, &c);
  p2rc(p, &sr, &sc);
  movePiece(l,r,c,sr,sc);
  if (pull) {
    int gr,gc;
    // compute box location
    gr = r-(sr-r);
    gc = c-(sc-c);
    // move box into former worker location
    movePiece(l,gr,gc,r,c);    
  }
  // update biostatistics
  updateStats(l);
  refresh();
  // fix move count
  MoveCount--;
  return 1;
}

// write a message
void message(char *msg)
{
  move(0,0);
  clrtoeol();
  mvstr(0,(COLS-strlen(msg))/2,msg);
  move(0,0);
}

// a curses extension: write string at [r,c], clear to end of line
void mvstr(int r, int c, char *s)
{
  move(r,c);
  while (*s) addch(*s++);
  clrtoeol(); // clear the tail of the line
}

// write important statistics to the screen
void updateStats(level *l)
{
  char buffer[80];
  long curTime = time(0);
  int deltaTime = curTime-l->startTime;
  double days,m,e,p;
  int seconds = (int)(deltaTime%60);
  int minutes = (int)((deltaTime/60)%60);
  int hours = (int)((deltaTime/3600)%24);
  int idays = (int)(deltaTime/86400);
  double workerSpeed;
  if (deltaTime)
    workerSpeed = MoveCount*0.1/deltaTime * 0.056818182; // see man units
  else
    workerSpeed = 0.0;

  sprintf(buffer,"Level: %d    Moves: %d    Time: %+d %d:%02d:%02d    Speed: %8.5lf mph",
	  l->levelNumber, MoveCount, idays, hours, minutes, seconds, workerSpeed);
  mvstr(LINES-3, (COLS-strlen(buffer))/2, buffer);

  // bioindicators
  days = (time(0)-BTime)/86400.0;
  p = sin(days/23.0*2.0*M_PI);
  m = sin(days/33.0*2.0*M_PI);
  e = sin(days/28.0*2.0*M_PI);
  sprintf(buffer,"Physical: %+8.6lf  Emotional: %+8.6lf  Mental: %+8.6lf",p,e,m);
  mvstr(LINES-2,(COLS-strlen(buffer))/2, buffer);
  move(LINES-1,0);
}

// Boss on Deck: print an emacs facade and duck for cover
void OMG(level *lv)
{
  FILE *f = fopen(OMGSCREEN,"r");
  int i,j,ch,l;
  char buffer[100];
  char *status = "-uu-:---F1 gdc.c        All L11     (C-wizard Abbrev)---";
  clear();
  // read the workscreen file, painting as many lines on screen as needed
  for (i = 0; i < LINES-2 && (0 != fgets(buffer,100,f)); i++) {
    l = strlen(buffer);
    for (j = 0; j < l; j++) {
      ch = buffer[j];
      if (isspace(ch)) ch = ' ';  // trash tabs and newlines
      mvaddch(i,j,ch);
    }
  }
  // now, the status line
  l = strlen(status);
  for (j = 0; j < COLS; j++) {
    if (j >= l) ch = '-';	// past the end of status line; fill with '-'
    else ch = status[j];	// within status line
    mvaddch(LINES-2,j,ch | A_REVERSE); // paint in reverse video
  }
  fclose(f);
  move(12,0); // move cursor to 13th line.  why not?
  refresh(); // paint facade
  // wait for the dust to clear
  getch();
  // repaint the level as it currently is
  clear();
  display(lv);
  updateStats(lv);
  refresh();
}

// print help screen
void help(level *lv)
{
  FILE *f = fopen(HELPSCREEN,"r");
  int i,j,l;
  char buffer[100];
  clear();
  // read the help file painting as many lines on screen as possible.
  for (i = 0; i < LINES && (0 != fgets(buffer,100,f)); i++) {
    l = strlen(buffer);
    for (j = 0; j < l; j++) {
      mvaddch(i,j,buffer[j]);
    }
  }
  fclose(f);
  move(0,0);
  refresh();
  getch();
  clear();
  display(lv);
  updateStats(lv);
  refresh();
}

// the main method
int main(int argc, char **argv)
{
  int currentLevelNumber;
  level *currentLevel;

  if (argc > 1) currentLevelNumber = atoi(argv[1]);
  else currentLevelNumber = 1;

  // start the curses screen manager
  initialize();
  
  // the play loop.  cranks once per level.
  while (currentLevelNumber <= MAXLEVEL) {
    currentLevel = readLevel(currentLevelNumber);
    display(currentLevel);
    play(currentLevel);
    currentLevelNumber++;
  }
  shutdown();
  return 0;
}
