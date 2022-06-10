/*
 * genListADT.h
 * Contrato del TAD Lista Lineal generica, cada elemento es void *
 * Posibilidades: 
 *    1)Lista ordenada, aceptando elementos repetidos.
 *    2)Lista desordenada a la cual se le pueden agregar elementos  
 *	al principio o al final. Acepta elementos repetidos.
 * Se proveen funciones de iteracion para recorrer la lista
 * POSIBLE MEJORA: que la lista reciba adem�s una funci�n de copia, que si
 *                 es NULL copia solamente los punteros recibidos (como hace
 *                 esta version).
 */

#ifndef GENLISTADT_H_
#define GENLISTADT_H_

typedef void * listElementT;

/* El siguiente tipo define el TAD Lista.
 * Se lo define como puntero a "cierta estructura"
 * para ocultar la real representacion de la misma
 */
typedef struct listCDT *listADT;


/*IMORTANTE: Si desea utilizar listas desordenadas, la función 
**compare debe retornar 1 en caso de recibir algún puntero NULL.
**En cambio si va a utilizar listas ordenadas,al recibir algún 
**punteros NULL debe retornar 0.
 *
 * Descripcion:	Inicializa la lista con la funcion de comparacion
 *              que no puede ser NULL
 * -------------------------------------------------------------------
 * Precondicion:		-
 * Postcondicion: Lista creada
 */
listADT NewList(int (*compare) (void*, void*));


/*IMPORTANTE: Si la lista es desordenada esta función no hace nada
 */
/* Funcion:  		Insert
 * Uso:  		Insert( list, myElement);
 * -------------------------------------------------------------------
 * Descripci�n: 	Inserta un elemento en la lista ordenada,
 *  			usando la funci�n de comparaci�n.
 * 			Si el elemento ya exist�a, lo ignora.
 * 			Si hay problemas en la alocaci�n de memoria
 * 			finaliza la aplicaci�n retornando 1, de lo
 *			contrario retorna 0.
 * -------------------------------------------------------------------
 * Precondicion:	Lista que sea valida, previamente creada
 *			myElement != NULL
 * Postcondicion: 	Lista con el nuevo elemento,si no estaba repetido
 *			ni era NULL (en ambos casos retornaría -1).
 */
char Insert(listADT list, listElementT element);


/*Inserta una ficha al final de una lista desordenada.
 *Si la lista es ordenada, esta función no hace nada
 *Retorna:
 *  1 - Si termino sin problemas
 *  0 - Si no hay suficiente memoria
 * -1 - Si el elemento es NULL.
 */
char EndInsert ( listADT list, listElementT element);


/*Inserta una ficha al principio de una lista desordenada.
 *Si la lista es ordenada, esta función no hace nada
 *Retorna:
 *  1 - Si termino sin problemas
 *  0 - Si no hay suficiente memoria
 * -1 - Si el elemento es NULL.
 */
char BeginInsert ( listADT list, listElementT element);


/* Funcion:  		Delete
 * Uso:  		Delete(list, myElement );
 * -------------------------------------------------------------------
 * Descripci�n: 	Elimina el elemento de la lista ordenada,
 *  			usando la funci�n de comparacion. Si el elemento
 * 			no existia devuelve 0, caso contrario 1
 * -------------------------------------------------------------------
 * Precondicion:	Lista que sea valida, previamente creada
 * Postcondicion: 	Lista sin el elemento
 */
int Delete(listADT list, listElementT element);


/* Funcion:  		ListIsEmpty
 * Uso:  		if ( ListIsEmpty( list ) ...
 * -------------------------------------------------------------------
 * Descripci�n: 	Devuelve 1 si la lista est� vac�a o
 * 			0 en caso contrario
 * -------------------------------------------------------------------
 * Precondicion:	Lista que sea valida, previamente creada
 * Postcondicion: 	- (lista invariante).
 */
int ListIsEmpty(listADT list);




/* Funcion:  		ElementBelongs
 * Uso:  		if ( ElementBelongs( list, 12.3 ))...
 * -------------------------------------------------------------------
 * Descripci�n: 	Devuelve 1 si el elemento pertenece a la lista
 * 			o 0 en caso contrario
 * -------------------------------------------------------------------
 * Precondicion:	Lista previamente creada
 * Postcondicion: 	- (lista invariante).
 */
int ElementBelongs(listADT list, listElementT element);



/* Funcion:  		ToBegin
 * Uso:  		ToBegin(list)
 * -------------------------------------------------------------------
 * Descripci�n: 	Coloca el iterador en el primer elemento de la lista
 * -------------------------------------------------------------------
 * Precondicion:	Lista no vac�a
 * Postcondicion: 	- (lista invariante).
 */
void ToBegin(listADT list);


/*IMPORTANTE: Esta función solo tiene efecto si la lista es desordenada
 */
/* Funcion:  		ToEnd
 * Uso:  		ToEnd(list)
 * -------------------------------------------------------------------
 * Descripci�n: 	Coloca el iterador en el último elemento de la lista
 * -------------------------------------------------------------------
 * Precondicion:	Lista no vac�a
 * Postcondicion: 	- (lista invariante).
 */
void
ToEnd( listADT list );


/* Funcion:  		NextElement
 * Uso:  		listADT list;
 *                      listElementT elem;
 * 			ToBegin(list)
 * 			while ( (elem = NextElement(list))  != NULL)
 * 			{
 * 				.....
 * 				.....
 * 			}
 * -------------------------------------------------------------------
 * Descripci�n: 	Retorna el siguiente elemento del iterador (no una copia)
 * 			Si ya no hay m�s elementos, retorna NULL
 *			Si se la invoca despu�s de Insert o Delete puede retornar
 *                      direcciones inv�lidas (referencias a elementos eliminados, etc.)
 * -------------------------------------------------------------------
 * Precondicion:	Lista creada
 * Postcondicion: 	Iterador apunta al siguiente elemento
 */
listElementT NextElement(listADT list);


/* Funcion: 		Size
 * Uso: 	
 * 		listADT list;
 *		listElementT elem;
 *		n = Size( list );
 * 		ToBegin(list)
 *		for(i=1; i <= n; i++)
 * 		{
 * 			elem = NextElement(list);
 * 			.....
 * 		}
 * -------------------------------------------------------------------
 * Descripci�n: 	Retorna la cantidad de nodos que hay en la lista
 *                      Puede usarse para iterar sobre la misma
 * -------------------------------------------------------------------
 * Precondicion:	Lista que sea valida.
 * Postcondicion: 	-
 */
int Size(listADT list);



/* Funcion: 		FreeList
 * Uso: 			FreeList( list );
 * -------------------------------------------------------------------
 * Descripci�n: 	Destruye la lista, liberando recursos alocados.
 * -------------------------------------------------------------------
 * Precondicion:	Lista que sea valida.
 * Postcondicion: 	-
 */
void FreeList(listADT list);

#endif /* GENLISTADT_H_ */
