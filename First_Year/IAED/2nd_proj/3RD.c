/*
80789 Joao Silveira
81105 Ana Costa
81478 Joana Godinho
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoes.h"

estado_geral State;

int main()
{
	char com[12];
	p_cabeca clientela;
	clientela=STinit();
	Pool_init();
	initEstado(&State);
	while (1)
	{
		scanf("%s",com);
		if (!strcmp(com,"cheque"))
		{
			long int valor,refe,refb,refc;
			scanf(" %ld %ld %ld %ld",&valor,&refe,&refb,&refc);
			trata_cheque(valor,refe,refb,refc,clientela);
		}
		else if (!strcmp(com,"processa"))
		{
			trata_processa(clientela);
		}
		else if (!strcmp(com,"processaR"))
		{
			long int refc;
			scanf(" %ld",&refc);
			trata_processaR(refc,clientela);
		}
		else if (!strcmp(com,"infocheque"))
		{
			long int refc;
			scanf(" %ld",&refc);
			info_cheque(refc);
		}
		else if (!strcmp(com,"infocliente"))
		{
			long int refcl;
			scanf(" %ld",&refcl);
			trata_infocliente(refcl, clientela);
		}
		else if (!strcmp(com,"info"))
		{
			trata_info(clientela);
		}
		else if(!strcmp(com,"sair"))
		{
			trata_sair(clientela);
			return 0;
		}
		else
			printf("ERRO: Comando desconhecido\n");
	}
	return 0;
}