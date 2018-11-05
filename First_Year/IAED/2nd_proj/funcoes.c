#include "funcoes.h"

extern estado_geral State;

void initEstado(estado_geral* state)
{
	state->clientes_ativ = 0;
	state->n_chepp = 0;
	state->v_chepp = 0;
}


void printEstado(estado_geral* state)
{
	printf("%d %d %ld\n", state->clientes_ativ, state->n_chepp, state->v_chepp);
}


void trata_cheque(long int valor,long int refe,long int refb,long int refc, p_cabeca clientela)
{
	cria_cheque(valor,refc,refe,refb);
	altera_clientes(refe,'e',valor, clientela, &State);
	altera_clientes(refb,'b',valor, clientela, &State);
	State.n_chepp++;
	State.v_chepp+=valor;
}


void trata_processa(p_cabeca clientela)
{
	cheque check;
	if (Pool_vazia())
		printf("Nothing to process\n");
	else 
	{
		check=Pool_saca();
		altera_infocliente(check.refe,check.val,'e',clientela, &State);
		altera_infocliente(check.refb,check.val,'b',clientela, &State);
		State.n_chepp--;
		State.v_chepp-=check.val;
	}
}


void trata_processaR(long int refc, p_cabeca clientela)
{
	link p_check;
	p_check = Pool_saca_ref(refc);
	if (p_check==NULL)
		printf("Cheque %ld does not exist\n",refc);
	else 
	{
		altera_infocliente((p_check->item).refe,(p_check->item).val,'e',clientela, &State);
		altera_infocliente((p_check->item).refb,(p_check->item).val,'b',clientela, &State);
		State.n_chepp--;
		State.v_chepp-=(p_check->item).val;
		free(p_check);
	}
}


void trata_infocliente(long int refcl, p_cabeca clientela)
{
	linkC a;	
	a=searchR(cab(clientela),refcl);
	printf("Cliente-info: ");
	printItemC(a->item);
}


void trata_info(p_cabeca clientela)
{
	if (State.clientes_ativ == 0)
		printf("No active clients\n");
	print_info_clientes(cab(clientela));
}


void trata_sair(p_cabeca clientela)
{
	Pool_limpa();
	freeR(cab(clientela));
	free(clientela);
	printEstado(&State);
}