CC=clang
CFLAGS=-Wall
OBJFLAG=-Wall -c

solx: solace.o lex.yy.o token.o
	$(CC) $(CFLAGS) lex.yy.o solace.o token.o -o solx

lex.yy.o: lex.yy.c
	$(CC) $(OBJFLAG) lex.yy.c

lex.yy.c: slex.l
	flex slex.l

solace.o: solace.c
	$(CC) $(OBJFLAG) solace.c

token.o: token.c
	$(CC) $(OBJFLAG) token.c

clean:
	rm lex.yy.c *.o solx