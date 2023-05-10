CC=clang
CFLAGS=-Wall
OBJFLAG=-Wall -c

solace: lex.yy.o solace.o
	$(CC) $(CFLAGS) lex.yy.o solace.o token.o -o solace

solace.o: solace.c
	$(CC) $(OBJFLAG) solace.c

token.o: token.c
	$(CC) $(OBJFLAG) token.c

lex.yy.o: lex.yy.c
	$(CC) $(OBJFLAG) lex.yy.c

lex.yy.c: slex.l
	flex slex.l

clean:
	rm lex.yy.c *.o