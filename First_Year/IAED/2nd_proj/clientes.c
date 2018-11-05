#include <stdlib.h>

#include "clientes.h"
#include "Item.h"

cliente cria_cliente(long int ref, char n, long int v_cheque, estado_geral* State)
{
	cliente client;
	client.ref=ref;
	if (n=='b')
	{
		client.nchB = 1;
		client.vchB = v_cheque;
		client.nchE = 0;
		client.vchE =0;
	}
	else
	{
		client.nchE = 1;
		client.vchE = v_cheque;
		client.nchB = 0;
		client.vchB =0;
	}
	State->clientes_ativ++;
	return client;
}

void altera_clientes(long int ref, char n, long int v_cheque, p_cabeca estrut, estado_geral* State)
{
	linkC a;
	cliente client;
	a=searchR(cab(estrut),ref); /*procura o cliente para ver se ele ja existe ou nao na arvore; o a e um ponteiro para um no*/
	if (a==NULL) /*caso em que o cliente nao existe na arvore*/
	{
		client = cria_cliente(ref,n,v_cheque, State);
		cab(estrut)=insertR(cab(estrut), newItemC(client));
	}
	else if (n=='b')
	{
		if ((a->item).nchB == 0 && (a->item).nchE == 0)
			State->clientes_ativ++;
		(a->item).nchB++;
		(a->item).vchB=(a->item).vchB+v_cheque;
	}
	else
	{
		if ((a->item).nchB == 0 && (a->item).nchE == 0)
			State->clientes_ativ++;
		(a->item).nchE++;
		(a->item).vchE=(a->item).vchE+v_cheque;	
	}
}

void altera_infocliente(long int ref,long int v_cheque,char n,p_cabeca estrut, estado_geral* State)
{
	linkC a;
	a=searchR(cab(estrut),ref);
	if (n=='b')
	{
		(a->item).nchB--;
		(a->item).vchB=(a->item).vchB-v_cheque;
	}
	else
	{
		(a->item).nchE--;
		(a->item).vchE=(a->item).vchE-v_cheque;
	}
	if ((a->item).nchB == 0 && (a->item).nchE == 0)
		State->clientes_ativ--;
}

void print_info_clientes(linkC h)
{
  if(h==NULL)
    return;
  print_info_clientes(h->l);
  if((h->item).nchE !=0 || (h->item).nchB !=0 )
  {
  	printf("*");
    printItemC(h->item);
  }
  print_info_clientes(h->r);
}