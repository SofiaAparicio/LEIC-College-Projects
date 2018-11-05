/*
 * list.h - definitions and declarations of the integer list 
 */

#ifndef LIST_TERMINAL_H
#define LIST_TERMINAL_H

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>



/* lst_terminal_iitem - each element of the list points to the next element */
typedef struct lst_terminal_iitem {
	int pid;
	struct lst_terminal_iitem *next;
} lst_terminal_iitem_t;

/* list_terminal */
typedef struct {
	lst_terminal_iitem_t * first;
} list_terminal;

list_terminal *terminais;

list_terminal* lst_terminal_new();

void lst_terminal_destroy();

void insert_new_terminal(int pid);

void remove_terminal(int pid);

int list_size();

void kill_em_all();

#endif 