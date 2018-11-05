#ifndef _funcoes_
#define _funcoes_

#include "Item.h"
#include "Pool.h"
#include "clientes.h"
#include "cheques.h"
#include "Pool.h"
#include "Item.h"
#include "arvore.h"

/*
initEstado: estado_geral* -> {}
Inicializa o estado geral.
*/
void initEstado(estado_geral*);

/*
printEstado:estado_geral* -> {}
Imprime o estado geral.
*/
void printEstado(estado_geral*);

/*
trata_cheque: long int, long int, long int, long int, p_cabeca -> {}
Cria e introduz um cheque na Pool, altera a informacao dos clientes, e atualiza os valores do estado geral.
*/
void trata_cheque(long int,long int,long int,long int, p_cabeca);

/*
trata_processa: p_cabeca -> {}
Processa o primeiro cheque da Pool, atualiza a informacao dos clientes e o estado geral.
*/
void trata_processa(p_cabeca);

/*
trata_processaR: long int, p_cabeca ->{}
Processa o cheque com uma dada referencia, altera a informacao dos clientes e o estado geral.
*/
void trata_processaR(long int, p_cabeca);

/*
trata_infocliente: long int, p_cabeca -> {}
Procura o cliente, com a refencia dada, e imprime a informacao deste.
*/
void trata_infocliente(long int,p_cabeca);

/*
trata_info: p_cabeca -> {}
Verifica se o cliente se encontra ativo, caso esteja, imprime a sua informacao.
*/
void trata_info(p_cabeca);

/*
trata_sair: p_cabeca -> {}
Liberta a Pool, a estrutura dos clientes e imprime o estado geral.
*/
void trata_sair(p_cabeca);

#endif