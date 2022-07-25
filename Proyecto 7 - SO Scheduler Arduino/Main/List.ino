   #include "List.h"

    Node fn, sn;

/* 
 *  list->e siempre es NULL, como una pila  
 */

struct Node* newList() {
  return &fn;  // RESERVE MEMORY
}

void addElem(void *e, struct Node *list) {
  Node *node = &sn; //(Node *) reserveMem( sizeof(Node) );
  node->e = e;
  node->prev = list->prev;
  list->prev = node;
}

struct Node* getFromLast(int i, struct Node *n) {
  if ( n == NULL || i < 0 )
    return NULL;
  int j = -1;
  while ( n->prev != NULL && j++ < i )
    n = n->prev;
  if ( j == i )
    return n;
  return NULL;
} 

struct Node* getNextTo(BOOL (*notEq)(void*, void*), void *e, struct Node *n) {
  Node *next = NULL;
  while ( n != NULL  && notEq(e,n->e) ) {
    next = n;
    n = n->prev;
  }
  if ( n == NULL )
    return NULL;
  return next;
}

void delElem(BOOL (*notEq)(void*, void*), void *e, struct Node *list) {
  Node *next = getNextTo(notEq, e, list);
  if ( next != NULL ) {
    next->prev = next->prev->prev;
    // Liberar memoria
  }
}
