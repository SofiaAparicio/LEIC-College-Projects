# Makefile for par-shell, version 1
# Sistemas Operativos, DEI/IST/ULisboa 2015-16

CFLAGS=-g -Wall -pedantic

all: par-shell par-shell-terminal fibonacci div0

par-shell: par-shell.o commandlinereader.o list.o list_terminal.o par-shell-terminal.o
	gcc -pthread -o par-shell par-shell.o commandlinereader.o list.o list_terminal.o

par-shell.o: par-shell.c
	gcc $(CFLAGS) -c par-shell.c

par-shell-terminal: par-shell-terminal.o commandlinereader.o par-shell.o
	gcc -o par-shell-terminal par-shell-terminal.o commandlinereader.o

par-shell-terminal.o: par-shell-terminal.c
	gcc $(CFLAGS) -c par-shell-terminal.c

commandlinereader.o: commandlinereader.c commandlinereader.h
	gcc $(CFLAGS) -c commandlinereader.c

list.o: list.c list.h
	gcc $(CFLAGS) -c list.c

list_terminal.o: list_terminal.c list_terminal.h
	gcc $(CFLAGS) -c list_terminal.c

fibonacci:
	gcc $(CFLAGS) -o fibonacci fibonacci.c

div0: div.c
	gcc $(CFLAGS) -o div0 div.c

run:
	clear
	./par-shell < i2.txt

clean:
	rm *.o par-shell fibonacci div0 par-shell-terminal
