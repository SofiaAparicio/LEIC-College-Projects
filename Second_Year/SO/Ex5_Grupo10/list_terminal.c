/*
 * list.c - implementation of the integer list functions 
 */


#include "list_terminal.h"

list_terminal* lst_terminal_new()
{
	list_terminal *list;
	list = (list_terminal*) malloc(sizeof(list_terminal));
	list->first = NULL;
	return list;
}


void lst_terminal_destroy()
{
	struct lst_terminal_iitem *item, *nextitem;

	item = terminais->first;
	while (item != NULL){
		nextitem = item->next;
		free(item);
		item = nextitem;
	}
	free(terminais);
}

void insert_new_terminal(int pid)
{
	lst_terminal_iitem_t *item;

	item = (lst_terminal_iitem_t *) malloc (sizeof(lst_terminal_iitem_t));
	item->pid = pid;
	item->next = terminais->first;
	terminais->first = item;
}

void remove_terminal(int pid)
{
	lst_terminal_iitem_t *item,*prev;
	item = terminais->first;
	if (item->pid == pid){
		terminais->first = item->next;
		free(item);
		return;
	}
	prev = item;
	item = item->next;
	while(item != NULL && prev != NULL){
		if (item->pid == pid){
			lst_terminal_iitem_t *temp = item;
			prev->next = item->next;
			free(temp);
			return;
		}
		prev = item;
		item = item->next;
	}
}

int list_size() {
	int i=0;
	lst_terminal_iitem_t *item,*prev;
	item = terminais->first;
	if (item != NULL){
		prev = item;
		item = item->next;
		i++;
		while(item != NULL && prev != NULL){
			i++;
			prev = item;
			item = item->next;
		}
	}
	return i;
}

void kill_em_all(){
	lst_terminal_iitem_t *item,*prev;
	item = terminais->first;
	if (item != NULL){
		/*printf("%d\n", item->pid);*/
		kill(item->pid, SIGINT);
		prev = item;
		item = item->next;
		while(item != NULL && prev != NULL){
			/*printf("%d\n", item->pid);*/
			kill(item->pid, SIGINT);
			prev = item;
			item = item->next;
		}
	}
}
