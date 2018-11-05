#include <stdlib.h>
#include "Item.h"
#include "Pool.h"

static link cabeca, cauda;

link cria_no(Item item, link next)
{
	link x = (link) malloc(sizeof(struct NoPool));

	x->item = item;
  	x->next = next;
 	return x;
}

void Pool_init()
{
	cabeca = NULL;
	cauda = NULL;
}

int  Pool_vazia()
{
	return cabeca==NULL;
}

void Pool_enfia(Item item)
{
 	if (Pool_vazia())
 	{
 		cabeca = (cauda = cria_no(item, cabeca));
 		return;
 	}

 	cauda->next = cria_no(item, cauda->next);
 	cauda = cauda->next;
}

Item Pool_saca() 
{
	Item item = cabeca->item;
	link t = cabeca->next;
	free(cabeca); 
	cabeca = t;
	return item;
}

void Pool_limpa()
{
	if (Pool_vazia()) return;
	while (cabeca != cauda)
	{
		Pool_saca();
	}
	Pool_saca();
}
link  Pool_fim(){
	return cauda;
}
link Pool_inicio(){
	return cabeca;
}
void altera_inicio(link x){
	if (cabeca != x)
		cabeca=x;
	else
		cabeca=NULL;
}

void altera_fim(link x){
	cauda=x;
}