#ifndef _POOL_H_
#define _POOL_H_


/*OBS: La estructura chip_t no es TAD porque el usuario necesita conocer los
**valores de la ficha.
 */
typedef struct chip_t{
 int x;
 int y;
}chip_t;

typedef struct poolCDT *poolADT; 


/*Crea un pozo con fichas desde (0,0) hasta (double_main,double_main)
**OBS: Tener en cuenta que para todo par ordenado (x,y), y>=x
 */
poolADT 
create_pool(int max_number);


/*Toma una ficha de pozo.
**
**Retorna en su nombre:
**	Un puntero a la ficha.
**	NULL - si no hay fichas retorna NULL
 */
chip_t *
get_chip(poolADT pool);


/*Devuelve una ficha en particular del pozo.
**
**Retorna en su nombre:
**	Un puntero a la ficha .
**	NULL - si la ficha no está en el pozo. 
**
**Recibe:
**	chip - una copia de la ficha que se desea sacar. 
 */
chip_t *
get_specific_chip(poolADT pool, chip_t chip);


/*Coloca una ficha en el pozo.
**
**Retorna en si nombre:
**	0 - si no hay memoria suficiente.
**	1 - si no hubo problemas.
**
**Recibe:
**	chip - una copia de la ficha que se desea colocar. 
 */
char 
put_chip(poolADT pool, chip_t chip);


/*Devuelve la cantidad de fichas que quedan en el pozo
 */
int
remaining_chips(poolADT pool);


/*Libera la memoria reservada para el pozo
 */
void 
free_pool(poolADT pool);

#endif
