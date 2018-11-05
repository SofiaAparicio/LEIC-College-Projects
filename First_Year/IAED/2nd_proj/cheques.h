#ifndef _cheques_
#define _cheques_

#include "Item.h"
#include "Pool.h"

#define cheque_ref(A) ((&A)->ref)
#define cheque_refE(A) ((&A)->refe)
#define cheque_refB(A) ((&A)->refb)
#define cheque_valor(A) ((&A)->val)
#define print_valor(A)	printf("%ld\n",A);
#define print_cheque(A) printf("Cheque-info: %ld %ld %ld --> %ld\n", (&A)->ref,(&A)->val,(&A)->refe, (&A)->refb)

/*
cria_cheque: long int,long int,long int,long int -> {}
Cria e introduz um novo cheque na Pool
*/
void cria_cheque(long int,long int,long int,long int);

/*
info_cheque: long int -> {}
Imprime a informacao de um cheque*/
void info_cheque(long int);

/*
Pool_saca_ref: long int -> link
Retira da Pool o cheque com a referencia dada
*/
link Pool_saca_ref(long int);

#endif