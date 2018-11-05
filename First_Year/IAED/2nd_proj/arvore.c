#include <stdlib.h>

#include "arvore.h"

p_cabeca STinit(){
  p_cabeca x=(p_cabeca) malloc(sizeof(struct cabeca));
  x->head=NULL;
  return x;
}

linkC NEW(ItemC item, linkC l, linkC r)
{
  linkC x = (linkC) malloc(sizeof(struct STnode));
  x->item = item;
  x->l = l;
  x->r = r;
  x->height=1;
  return x;
}

int height(linkC h){
  if (h == NULL) return 0;
  return h->height;
}

/*rotacao simples para a esquerda*/
linkC rotL(linkC h) 
{
  int height_left, height_right;
  linkC x = h->r;
  h->r = x->l;
  x->l = h;

  height_left = height(h->l);
  height_right = height(h->r);
  h->height = height_left > height_right ? height_left + 1 : height_right + 1;

  height_left = height(h->l);
  height_right = height(x->r);
  x->height = height_left > height_right ? height_left + 1 : height_right + 1;

  return x;
}

/*rotacao simples para a direita*/
linkC rotR(linkC h) 
{
  int height_left, height_right;
  linkC x = h->l;
  h->l = x->r;
  x->r = h;

  height_left = height(h->l);
  height_right = height(h->r);
  h->height = height_left > height_right ? height_left + 1 : height_right + 1;

  height_left = height(x->l);
  height_right = height(h->r);
  x->height = height_left > height_right ? height_left + 1 : height_right + 1;

  return x; 
}

linkC rotLR(linkC h) /*rotação dupla esquerda direita*/
{
  if (h==NULL) return h;
  h->l=rotL(h->l);
  return rotR(h); 
}

linkC rotRL(linkC h) /*rotação dupla direita esquerda*/
{
  if (h==NULL) return h;
  h->r=rotR(h->r); 
  return rotL(h); 
}

/* balance factor, i.e., diferenca entre as alturas */
int balance(linkC h) { 
  if (h == NULL) return 0; 
  return height(h->l)-height(h->r); 
} 

/*equilibra a arvore*/
linkC AVLbalance(linkC h) {    
  int balanceFactor, height_left, height_right;

  if (h==NULL) return h;

  balanceFactor = balance(h); 

  if (balanceFactor > 1) { 
    if (balance(h->l) > 0) h = rotR(h); 
    else                   h = rotLR(h);  
  } 
  else if (balanceFactor < -1) { 
    if (balance(h->r) < 0) h = rotL(h); 
    else                   h = rotRL(h); 
  } else {
    height_left = height(h->l);
    height_right = height(h->r);
    h->height = height_left > height_right ?  height_left + 1 : height_right + 1;
  }

  return h; 
} 

linkC searchR(linkC h, Key c)
{
  if (h==NULL)
    return NULL;
  else if (key(h->item)==c)
    return h;
  else if (less(c,(key(h->item))))
    return searchR(h->l,c);
  else
    return searchR(h->r,c);
}

/*ItemC STsearch(linkC head, Key c)
{
  return searchR(head,c);
}*/


/*void STinsert(linkC*head,ItemC item) 
{
  *head=insertR(*head,item);
}*/
/*insere um no na arvore*/

linkC insertR(linkC h, ItemC item) 
{
  if (h == NULL) 
    return NEW(item, NULL, NULL);
  if (less(key(item),key(h->item)))    /*less(key(item), key(h->item)))*/
    h->l = insertR(h->l, item);
  else 
    h->r = insertR(h->r, item);

  h = AVLbalance(h);

  return h;
}


/*void sortR(linkC h)
{
  if(h==NULL)
    return;
  sortR(h->l);
  if(h->item)
    printItemC(h->item);
  sortR(h->r);
}*/

/*void sortR(linkC h void (*visit) (ItemC))
{
  if(h==NULL)
    return;
  sortR(h->l,visit);
  visit(h->item);
  sortR(h->r,visit);
}*/

/*void STsort(linkC head, void(*visit) (ItemC))
{
  sortR(head,visit);
}*/

linkC max(linkC h) {
 if (h==NULL || h->r==NULL) return h;
 else return max(h->r);
}

int count(linkC h){
 if (h==NULL) return 0;
 else return count(h->r) + count(h->l) + 1;
}

int STcount(linkC head){
 return count(head);
}

/*void STdelete(linkC*head, Key k){
 *head = deleteR(*head, k);
}*/

/*para remover um no da arvore*/
linkC deleteR(linkC h, Key k) {
  ItemC x;

  if (h==NULL) return h;    
  else if (less(k, key(h->item))) h->l=deleteR(h->l,k);
  else if (less(key(h->item), k)) h->r=deleteR(h->r,k) ;
  else{
    if (h->l != NULL && h->r != NULL){
      linkC aux = max(h->l);

      x = h->item;
      h->item = aux->item;
      aux->item=x;

      h->l = deleteR(h->l, key(aux->item));
    } else {
      linkC aux=h;
      if (h->l == NULL && h->r == NULL) h = NULL;
      else if (h->l == NULL) h = h->r;
      else h = h->l;
      free(aux);
    }
  } 
  h = AVLbalance(h);

  return h;
}

linkC freeR (linkC h)
{
  if (h==NULL)
    return h;
  h->l=freeR(h->l);
  h->r=freeR(h->r);
  return deleteR (h,key(h->item));
}

/*void freeR(linkC)
{

}*/