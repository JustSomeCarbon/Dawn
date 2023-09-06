CC=clang
CFLAGS=-Wall -ggdb3
OBJFLAG=-Wall -ggdb3 -c

solx: solace.o sgram.tab.o lex.yy.o tree.o
	$(CC) $(CFLAGS) sgram.tab.o lex.yy.o tree.o solace.o -o solx

solace.o: solace.c tree.h sgram.tab.h
	$(CC) $(OBJFLAG) solace.c

tree.o: tree.c tree.h
	$(CC) $(OBJFLAG) tree.c

lex.yy.o: lex.yy.c
	$(CC) $(OBJFLAG) lex.yy.c

lex.yy.c: slex.l tree.h sgram.tab.h
	flex slex.l

sgram.tab.o: sgram.tab.c sgram.tab.h
	$(CC) $(OBJFLAG) sgram.tab.c

sgram.tab.c sgram.tab.h: sgram.y
	bison -d -v sgram.y


clean:
	rm lex.yy.c *.tab.c *.tab.h *.o solx *.output