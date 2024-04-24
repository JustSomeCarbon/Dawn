CC=clang
CFLAGS=-Wall -ggdb3
OBJFLAG=-Wall -ggdb3 -c

solx: solace.o sgram.tab.o lex.yy.o tree.o symfunclib.o symtable.o
	$(CC) $(CFLAGS) sgram.tab.o lex.yy.o tree.o symfunclib.o symtable.o solace.o -o solx

solace.o: solace.c tree.h sgram.tab.h
	$(CC) $(OBJFLAG) solace.c

tree.o: tree.c tree.h
	$(CC) $(OBJFLAG) tree.c

symfunclib.o: symfunclib.c symtable.h tree.h sgram.tab.o
	$(CC) $(OBJFLAG) symfunclib.c

symtable.o: symtable.c symfunclib.o symtable.h tree.h
	$(CC) $(OBJFLAG) symtable.c

lex.yy.o: lex.yy.c
	$(CC) $(OBJFLAG) lex.yy.c

lex.yy.c: slex.l tree.h sgram.tab.h
	flex slex.l

sgram.tab.o: sgram.tab.c sgram.tab.h
	$(CC) $(OBJFLAG) sgram.tab.c

sgram.tab.c sgram.tab.h: sgram.y
	bison -d -v sgram.y


clean:
	rm lex.yy.c *.tab.c *.tab.h *.o *.output solx