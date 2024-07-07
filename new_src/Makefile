CC=clang
CFLAGS=-Wall -ggdb3
OBJFLAG=-Wall -ggdb3 -c

anvl: artifice.o agram.tab.o lex.yy.o tree.o symfunclib.o symtable.o err.o
	$(CC) $(CFLAGS) agram.tab.o lex.yy.o tree.o symfunclib.o symtable.o err.o solace.o -o solx

artifice.o: artifice.c tree.h agram.tab.h
	$(CC) $(OBJFLAG) solace.c

tree.o: tree.c tree.h
	$(CC) $(OBJFLAG) tree.c

lex.yy.o: lex.yy.c
	$(CC) $(OBJFLAG) lex.yy.c

lex.yy.c: alex.l tree.h agram.tab.h
	flex alex.l

agram.tab.o: agram.tab.c agram.tab.h
	$(CC) $(OBJFLAG) agram.tab.c

agram.tab.c agram.tab.h: agram.y
	bison -d -v agram.y


clean:
	rm lex.yy.c *.tab.c *.tab.h *.o *.output solx