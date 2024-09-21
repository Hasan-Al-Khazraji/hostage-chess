CC = clang
CFLAGS = -std=c99 -Wall -pedantic

all: test1

hclib.o: hclib.c hclib.h
	$(CC) $(CFLAGS) -c hclib.c -o hclib.o

test1.o: test1.c hclib.h
	$(CC) $(CFLAGS) -c test1.c -o test1.o

test1: test1.o hclib.o
	$(CC) test1.o hclib.o -o test1

clean:
	rm -f *.o test1