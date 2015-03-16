sokoban:	sokoban.c sokoban.h win.s
	gcc -Wall -g -o sokoban sokoban.c win.s -lncurses -lm

old:	csokoban

csokoban:	sokoban.c sokoban.h win.c
	gcc -Wall -g -o csokoban sokoban.c win.c -lncurses -lm

clean:	
	@rm -rf sokoban.o win.o *~ *.dSYM
	@echo Made clean.

realclean:	clean
	@rm -f sokoban csokoban
	@echo Really.

love:
	@echo Not war.