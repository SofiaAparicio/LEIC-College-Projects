/*
 * list.c - implementation of the integer list functions 
 */


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "list.h"



list_t* lst_new()
{
	list_t *list;
	list = (list_t*) malloc(sizeof(list_t));
	list->first = NULL;
	return list;
}


void lst_destroy(list_t *list)
{
	struct lst_iitem *item, *nextitem;

	item = list->first;
	while (item != NULL){
		nextitem = item->next;
		free(item);
		item = nextitem;
	}
	free(list);
}


void insert_new_process(list_t *list, int pid, time_t starttime)
{
	lst_iitem_t *item;

	item = (lst_iitem_t *) malloc (sizeof(lst_iitem_t));
	item->pid = pid;
	item->starttime = starttime;
	item->endtime = 0;
	item->next = list->first;
	list->first = item;
}


void update_terminated_process(list_t *list, int pid, int status, time_t endtime)
{
	lst_iitem_t *item;
	for (item = list->first; item != NULL; item = item->next){
		if (item->pid == pid){
			item->status = status;
			item->endtime = endtime;
			item->runtime = difftime(item->endtime, item->starttime);
			break;
		}
	}
}


void lst_print(list_t *list)
{
	lst_iitem_t *item;

	item = list->first;
	
	/* while(1){ */ /* use it only to demonstrate gdb potencial */
	while (item != NULL){	
		if (WIFEXITED(item->status))
			printf("pid: %d exited normally; status = %d; runtime = %.0f seconds\n", item->pid, WEXITSTATUS(item->status), item->runtime);
		else
			printf("pid: %d terminated without calling exit; runtime = %.0f seconds\n", item->pid, item->runtime);
		item = item->next;
	}
	printf("-- end of program.\n");
}
