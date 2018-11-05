#ifndef _ST_
#define _ST_

#include <stdlib.h>
#include <stdio.h>

#include "Item.h"

typedef struct STnode* linkC;

typedef struct cabeca* p_cabeca;

struct cabeca{
	linkC head;
};

struct STnode {
  ItemC item;
  linkC l, r;
  int height; /* altura do no */
};

p_cabeca STinit(); /*Inicia a cabeca*/
linkC freeR (linkC); /*Elimina a informacao presente na arvore*/
linkC deleteR(linkC, Key); /*Remove um no da arvore*/
/*int STcount(linkC); */
linkC insertR(linkC, ItemC); /* Insere um elemento na arvore*/
linkC searchR(linkC, Key); /*Procura o elemento com a Key pretendida*/

#endif