#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"pool.h"


typedef struct poolCDT{
  int 	 remaining_chips;
  chip_t	*chips;
}pool_t;


/* =========FUNCIONES AUXILIARES =============================== */

/*Se coloca en el poso todas las fichas desde (0,0) hasta
**(max_number,max_number) de forma ordenada
**
**Recibe:
**	max_number -  máximo numero posible en una ficha.
 */
static void 
chips(pool_t *pool, char max_number);


/*Mezcla las fichas del pozo
 */
static void 
mix_chips(pool_t *pool);


/*Retorna la cantidad de fichas que habrán para un determinado 
**"máximo numero de ficha".
 */
static int 
number_of_chips(int max_number);


/* ============================================================== */


/*Se Pide memoria para crear el pozo, y se llama a las funciones encargadas
**de llenarlo y mezclarlo.
**
**Retorna en su nombre:
**	un puntero al pozo.
 */
poolADT 
create_pool(int max_number)
{
  pool_t *pool;
  int amount = number_of_chips(max_number+1);

  pool = (pool_t*) malloc(sizeof(pool_t));
  if(pool == NULL)
    return NULL;

  pool->chips = (chip_t*) malloc(sizeof(chip_t) * amount);
  if(pool->chips == NULL){
    free(pool);
    return NULL;
  }

  chips(pool, max_number);
  pool->remaining_chips = amount;
  mix_chips(pool);
  return pool;
}


/*Libera la memoria recervada para el pozo.
 */
void 
free_pool(poolADT pool)
{
  free(pool->chips);
  free(pool);
}


/*Retorna en su nombre una ficha aleatoria del pozo
 */
chip_t *
get_chip(poolADT pool)
{
  if(remaining_chips(pool) == 0)
    return NULL;
  return &pool->chips[--pool->remaining_chips];
}


/*Retorna en su nombre una ficha especÃ­fica del pozo. Si la ficha no se 
**encuentra retorna NULL
 */
chip_t *
get_specific_chip(poolADT pool, chip_t chip)
{
  int i, size;
  chip_t aux;
  
  if((size = remaining_chips(pool)) == 0)
    return NULL;
  
  if(chip.x > chip.y){
    i = chip.y;
    chip.y = chip.x;
    chip.x = i;
  }
  
  for(i = 0; i < size && (pool->chips[i].x != chip.x ||
    pool->chips[i].y != chip.y) ; i++);

  if(i < size){
    aux = pool->chips[--pool->remaining_chips];
    pool->chips[pool->remaining_chips] = pool->chips[i];
    pool->chips[i] = aux;
    return &pool->chips[pool->remaining_chips];
  }
    
  return NULL;
}


/*Coloca una ficha en el pozo.
 */
char 
put_chip(poolADT pool, chip_t chip)
{
  chip_t *aux;
  aux =(chip_t *)realloc(pool->chips,sizeof(chip_t)*(pool->remaining_chips+1));
  if(aux == NULL)
    return 0;
  pool->chips = aux;
  pool->chips[pool->remaining_chips++] = chip;
  return 1;
}

/*Debuelve la cantidad de fichas que quedan en el pozo
 */
int 
remaining_chips(poolADT pool)
{
  return pool->remaining_chips;
}

/*Coloco en el poso todas las fichas desde (0,0) hasta
**(max_number,max_number) de forma ordenada
 */
static void 
chips(poolADT pool,char max_number)
{
  int i,j,k;
  for(i = 0, k = 0; i <= max_number; i++)
    for(j = i; j <= max_number; j++){
      pool->chips[k].x = i;
      pool->chips[k++].y = j;
    }
}


/*Mezcla las fichas del pozo
 */
static void 
mix_chips(pool_t *pool)
{
    int i,j;
    chip_t chip;

    srand((int)time(NULL));
    for(i = 0; i < pool->remaining_chips; i++){
    j = rand() % pool->remaining_chips;
    chip = pool->chips[i];
    pool->chips[i] = pool->chips[j];
    pool->chips[j] = chip;
  }
}

/*Retorna la cantidad de fichas que habran para un determinado 
**"máximo numero de ficha"
 */
static int 
number_of_chips(int max_number)
{
  if(max_number == 1)
	return 1;
  return max_number + number_of_chips(max_number-1);
}
