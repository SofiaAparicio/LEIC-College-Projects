#include <stdlib.h>

#include "cheques.h"
#include "Pool.h"
#include "Item.h"

void cria_cheque(long int val,long int ref,long int refe,long int refb)
{
	cheque check;
	Item check_item;
	check.ref = ref;
	check.refe = refe;
	check.refb = refb;
	check.val=val;
	check_item = newItem(check);
	Pool_enfia(check_item);
}

link Pool_saca_ref(long int refer)  /*x e ponteiro para um no; x->item e um cheque*/
{
	link x, aux = NULL, inicio_est = Pool_inicio(), fim_est = Pool_fim();
	x = inicio_est;
	if (!Pool_vazia())
	{
		for(;x!=fim_est && cheque_ref(x->item)!= refer;aux=x,x=x->next);
	
		if (cheque_ref(x->item) == refer)
		{
			if (aux != NULL && x!=fim_est) aux->next = x->next;
			else if(x!= inicio_est && x==fim_est)
			{
				aux->next = NULL;
				altera_fim(aux); /*caso em que o elemento a remover e o ultimo*/
			}
			else altera_inicio(x->next);    /*caso em que o elemento a remover esta no inicio*/
			return x;
		}
		else return NULL;
	}
	else return NULL;
}

void info_cheque(long int refer)
{
	link x = Pool_inicio(), fim_est = Pool_fim();
	for(;x!=fim_est && cheque_ref(x->item)!= refer;x=x->next);
	if (cheque_ref(x->item) == refer)
	{
		print_cheque(x->item);
	}
	else printf("Foi sacado!!");	
}
