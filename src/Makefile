CC=clang
CFLAGS=-Wall -ggdb3
OBJFLAG=-Wall -ggdb3 -c

solx: solace.o lex.yy.o sgram.tab.o tree.o
	$(CC) $(CFLAGS) lex.yy.o sgram.tab.o solace.o -o solx

tree.o: tree.c tree.h
	$(CC) $(OBJFLAG) tree.c

solace.o: solace.c tree.h sgram.tab.h
	$(CC) $(OBJFLAG) solace.c

lex.yy.o: lex.yy.c
	$(CC) $(OBJFLAG) lex.yy.c

sgram.tab.o: sgram.tab.c
	$(CC) $(OBJFLAG) sgram.tab.c

lex.yy.c: slex.l sgram.tab.c
	flex slex.l

sgram.tab.c sgram.tab.h: sgram.y
	bison -d -Wother -v sgram.y


clean:
	rm lex.yy.c sgram.tab.c sgram.tab.h *.o solx *.output