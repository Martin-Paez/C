/*
 * genlistADT.c
 *
 */

/* Version Iterativa de listas con header */

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

#define OUT_OF_ORDER_LIST	1

struct listCDT
{
	int (*fComp) (void *, void *);	/* funcion de comparacion */
	struct listNode * first;	/* puntero al primer nodo de la lista */
	struct listNode * actual;	/* puntero al actual, para iterar */
	struct listNode * end;		/* puntero al último nodo de la lista */
	int size;			/* cantidad de nodos */
};

typedef struct listNode
{
	listElementT data;
	struct listNode * next;
}listNode;


listADT
NewList( int (*fComp) (void *, void *) )
{
	listADT l = malloc(sizeof( struct listCDT));
	if ( l == NULL)
		return NULL;
	l->fComp = fComp;
	l->first = NULL;
	l->actual = NULL;
	l->size = 0;
	return l;
}


int
ListIsEmpty( listADT list)
{
	return list->size == 0;
	/* 
	return list->first == NULL
	*/
}


void
ToBegin( listADT list )
{
	list->actual = list->first;
}


void
ToEnd( listADT list )
{
	if( (*list->fComp)(NULL, NULL) == OUT_OF_ORDER_LIST )
	      list->actual = list->end;
}



listElementT
NextElement( listADT list)
{
	listNode * actual;
	
	if ( list->actual == NULL )
		return NULL;
	
	actual = list->actual;
	list->actual = actual->next;
	
	return actual->data;
}


int
ElementBelongs( listADT list, listElementT element)
{
	listNode * node;
	int cmp;
	node = list->first;

	/* Como no sabemos la complejidad de la funcion de comparacion, conservamos 
	** el resultado en una variable.
	*/
	while( node != NULL &&   (cmp = (*list->fComp) (node->data, element)) == -1)
		node = node->next;

	return node != NULL && cmp == 0;
}


/* Funcion auxiliar para insertar en la lista de nodos, en forma analoga
** a insertar en una lista lineal sin header
*/
static int
InsertNode( listNode ** list, listElementT element, int (*f)(void *, void*))
{
  int comp;
	/* Inserto al final o delante del actual porque es mayor */
	if( *list == NULL || (comp = (*f)((*list)->data, element)) >= 0 )
	{
		listNode * auxi = malloc(sizeof( listNode ));
		if (auxi == NULL)
			return 0;
		auxi->next = *list;
		auxi->data = element;
		*list = auxi;
		return 1;
	}


	/* Si no es vacia ni mayor verificar si es igual (no se insertan repetidos) */
	if( comp == 0 )
		return -1;

	/* El elemento actual es menor */
	return InsertNode( &((*list)->next), element, f);


}


#define OUT_OF_ORDER_LIST	1
char
Insert( listADT list, listElementT element)
{
	if (element == NULL)
		return -1;
	
	if( (*list->fComp)(NULL, element) != OUT_OF_ORDER_LIST )
	  list->size += InsertNode(&(list->first), element, list->fComp);

	return 1;
}


char BeginInsert ( listADT list, listElementT element)
{
	if (element == NULL)
		return -1;
	
	if( (*list->fComp)(NULL, element) == OUT_OF_ORDER_LIST )
	{
	  	  listNode * auxi = malloc(sizeof( listNode ));
		  if (auxi == NULL)
			  return 0;
		  auxi->next = list->first;
		  auxi->data = element;
		  list->first = auxi;
		  if(list->end == NULL)
		    list->end = auxi;
		  list->size++; 
		  return 1;
	}
	return 1;
}


char EndInsert ( listADT list, listElementT element)
{
	if (element == NULL)
		return -1;
	
	if( (*list->fComp)(NULL, element) == OUT_OF_ORDER_LIST )
	{
	  	  listNode * auxi = malloc(sizeof( listNode ));
		  if (auxi == NULL)
			  return 0;
		  auxi->next = NULL;
		  auxi->data = element;
		  if(list->end != NULL)
		    list->end->next = auxi;
		  if(list->first == NULL)
		    list->first = auxi;
		  list->end = auxi;
		  list->size++; 
		  return 1;
	}
	return 1;
}


static int
DeleteNode( listNode **list, listElementT element, int (*f)(void *, void*))
{
  int comp;
	if( *list == NULL || (comp = (*f)((*list)->data, element)) == 1 )
		return 0;

	if( comp == 0 )
	{
		listNode * aux = *list;
		*list = (*list)->next;
		free(aux);
		return 1;
	}

	/* El elemento actual es menor */
	return DeleteNode( &((*list)->next), element, f);

}

int
Delete( listADT list, listElementT element)
{
	int res;
	res = DeleteNode(&(list->first), element, list->fComp);
	list->size -= res;
	return res;
}


int
Size( listADT list )
{
	return list->size;
}

void
FreeList( listADT list)
{
	listNode * auxi, * node = list->first;

	while ( node != NULL )
	{
		auxi = node->next;
		free(node);
		node = auxi;
	}

	/* Liberamos el header */
	free(list);
}
