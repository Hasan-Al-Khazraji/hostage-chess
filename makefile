CC = clang
CFLAGS = -Wall -std=c99 -pedantic
PYTHON_INCLUDE = /usr/include/python3.11/
PYTHON_LIB = /usr/lib/python3.11/config-3.11-x86_64-linux-gnu
LIBS = -lm -lpython3.11 -lhclib

all: libhclib.so main hclib_wrap.o _hclib.so hclib.py hclib_wrap.c

clean:
	rm -f *.o *.so main

libhclib.so: hclib.o
	$(CC) hclib.o -shared -o libhclib.so

hclib.o: hclib.c hclib.h
	$(CC) $(CFLAGS) -c hclib.c -fPIC -o hclib.o

main.o: test1.c hclib.h
	$(CC) $(CFLAGS) -c test1.c -o main.o

main: test1.o libhclib.so
	$(CC) test1.o -L. -lhclib -o main

hclib_wrap.o: hclib_wrap.c
	$(CC) $(CFLAGS) -c hclib_wrap.c -I$(PYTHON_INCLUDE) -fPIC -o hclib_wrap.o

_hclib.so: hclib_wrap.o libhclib.so
	$(CC) $(CFLAGS) -shared hclib_wrap.o -L. -L$(PYTHON_LIB) $(LIBS) -o _hclib.so

hclib_wrap.c: hclib.i
	swig -python hclib.i