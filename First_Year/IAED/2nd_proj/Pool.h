#ifndef _pool_
#define _pool_

#include "Item.h"

typedef struct NoPool* link;

struct NoPool {
  Item item;
  link next;
};


link cria_no(Item, link); /*Cria um novo no com a informacao dada*/
void Pool_init(); /*Inicializa a Pool dos elementos*/
int  Pool_vazia(); /* Verifica se nao ha elementos na Pool*/
void Pool_enfia(Item); /*Introduz um novo elemento na Pool */
Item Pool_saca(); /*Retira o primeiro elemento da Pool*/
void Pool_limpa(); /*Apaga a informacao contida na Pool*/
link Pool_inicio(); /*Retorna o inicio da Pool*/
link Pool_fim(); /*Retorna o fim da Pool*/
void altera_inicio(link); /*Modifica o inicio da Pool*/
void altera_fim(link); /*Modifica o fim da Pool*/


#endif