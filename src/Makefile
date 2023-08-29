CC=gcc
CFLAGS=-Wall -ggdb3
OBJFLAG=-Wall -ggdb3 -c

solx: solace.o y.tab.o lex.yy.o tree.o
	$(CC) $(CFLAGS) y.tab.o lex.yy.o solace.o -o solx

tree.o: tree.c tree.h
	$(CC) $(OBJFLAG) tree.c

solace.o: solace.c tree.h y.tab.h
	$(CC) $(OBJFLAG) solace.c

lex.yy.o: lex.yy.c
	$(CC) $(OBJFLAG) lex.yy.c

lex.yy.c: slex.l tree.h y.tab.h
	flex slex.l

y.tab.o: y.tab.c y.tab.h
	$(CC) $(OBJFLAG) y.tab.c

y.tab.c y.tab.h: sgram.y
	yacc -d -Wother -v sgram.y


clean:
	rm lex.yy.c *.tab.c *.tab.h *.o solx *.output