# Makefile for par-shell, version 1
# Sistemas Operativos, DEI/IST/ULisboa 2015-16

CFLAGS=-g -Wall -pedantic

all: par-shell fibonacci

par-shell: par-shell.o commandlinereader.o list.o
	gcc -pthread -o par-shell par-shell.o commandlinereader.o list.o

par-shell.o: par-shell.c
	gcc $(CFLAGS) -c par-shell.c

commandlinereader.o: commandlinereader.c commandlinereader.h
	gcc $(CFLAGS) -c commandlinereader.c

list.o: list.c list.h
	gcc $(CFLAGS) -c list.c

fibonacci:
	gcc $(CFLAGS) -o fibonacci fibonacci.c

clean:
	rm *.o par-shell fibonacci
