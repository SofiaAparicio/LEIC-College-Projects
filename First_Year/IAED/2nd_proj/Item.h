#ifndef _ITEM_H_
#define _ITEM_H_

#include <stdio.h>

/*Cheques*/

typedef struct {
	int clientes_ativ, n_chepp;
	long int v_chepp;
}estado_geral;

typedef struct {
	long int ref, val, refe, refb;
}cheque;

typedef cheque Item;

#define newItem(A) (A)
#define print_Item(A) print_cheque(A)
#define cab(A) (A->head)

/*-----------------------------------------------------------------------------------------------------*/

/*Clientes*/

typedef struct {
	long int ref, vchE, vchB;
	int nchE, nchB;
}cliente;

typedef cliente ItemC;
typedef long int Key;

#define key(A) (A.ref)
#define less(a,b) (a<b)
#define newItemC(A) (A)
#define printItemC(A) printf("%ld %d %ld %d %ld\n",A.ref, A.nchE, A.vchE, A.nchB, A.vchB)


#endif