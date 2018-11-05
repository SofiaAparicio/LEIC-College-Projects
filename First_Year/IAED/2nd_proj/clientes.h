#ifndef _clientes_
#define _clientes_

#include "arvore.h"
#include "Item.h"


#define cliente_ref(A) (A->ref)
#define print_cliente(A) printItemC(A)

/*
print_info_clientes: linkC -> {}
Precorre a estrutura e impime a informacao de todos os clientes ativos
*/
void print_info_clientes(linkC);

/*
cria_cliente: long int, char, long int, *estado_geral -> cliente
Cria um cliente e inicializa a informacao do mesmo
*/
cliente cria_cliente(long int, char, long int, estado_geral*);

/*
altera_clientes: long int, char, long int, p_cabeca, estado_geral* -> {}
Para a adicao de cheques cria ou altera a informacao do cliente com a referencia dada, altera a estrutura e atualiza o estado geral
*/
void altera_clientes(long int, char, long int, p_cabeca, estado_geral*);

/*
altera_infocliente: long int,long int,char,p_cabeca, estado_geral* -> {}
Apos processar um cheque, cria ou altera a informacao do cliente com a referencia dada, altera a estrutura e atualiza o estado geral
*/
void altera_infocliente(long int,long int,char,p_cabeca, estado_geral*);

#endif