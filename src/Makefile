CC=clang
CFLAGS=-Wall -ggdb3
OBJFLAGS=-Wall -ggdb3 -c

anvl: solace.o slex.o token.o cleanup.o solerr.o
	$(CC) $(CFLAGS) solace.o

solace.o: solace.c solace.h solerr.o cleanup.o
	$(CC) $(OBJFLAGS) solace.c

token.o: token.c slex.h solerr.o cleanup.o
	$(CC) $(OBJFLAGS) token.c

slex.o: slex.c slex.h solerr.o
	$(CC) $(OBJFLAGS) slex.c

cleanup.o: cleanup.c cleanup.h solerr.o
	$(CC) $(OBJFLAGS) cleanup.c

solerr.o: solerr.c solerr.h
	$(CC) $(OBJFLAGS) solerr.c


clean:
	rm anvl *.o