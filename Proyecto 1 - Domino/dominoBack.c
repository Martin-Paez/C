#include <stdio.h>
#include <stdlib.h>
#include "pool.h"
#include "list.h"
#include "dominoBack.h"


/*Utilizadas para indicar la conveniencia de jugar una ficha
**en determinada posici n.
 */
#define _PLAY_RIGHT_GUARANTEED		 2
#define _PLAY_LEFT_GUARANTEED		 1
#define _PLAY_LEFT_NOT_GUARANTEED	-1
#define	_PLAY_RIGHT_NOT_GUARANTEED	-2

/*Para el uso de las funciones de comparaci n
 */
#define _EQUAL				  0
#define _A_HIGHER_THAN_B		  -1
#define _A_LOWER_THAN_B			  1

/*Para determinar estados.
 */
#define _LOOP				 10
#define	_CONTINUE			 1

/*Hacen referencia a las fichas del tablero y su posici n en el mismo
 */
#define _BEST				 LEFT
#define _BOTH_SIDES			 3
#define _NONE_SIDE			 4

/*Hace referencia al valor izquierdo de una ficha, la cual esta formada
**por un par ordenado de numeros (x,y).
 */
#define	X				 0


/*Modela a los jugadores*/
typedef struct player_t{
  int    	penalty;
  listADT	chips;
}player_t;

/*Modela el tablero*/
typedef struct board_t{
  chip_t	*left;		/*Ficha a la izq del tablero*/
  chip_t	*right;		/*Ficha a la der del tablero*/
  listADT	chips;
}board_t;

/*Utilizados para implementar la funci n undo_command()*/
typedef enum {CHIP_IN, CHIP_OUT, POOL} undo_t;

/*Estructura del juego*/
typedef struct gameCDT{
  int			game_mode;
  int			*pc_counter_chips;	/*Solo para la m quina*/
  int			*counter_chips;
  int 		max_number;
  player_index_t	turn;
  undo_t		undo;
  chip_t		*last_taked_chip;	/*Solo para el jugador*/ 
  poolADT 		pool;
  struct board_t	board;
  struct player_t players[NUMBER_OF_PLAYERS];
} gameCDT;


/* =========FUNCIONES AUXILIARES =============================== */

/*Crea las manos de los jugadores e inicializa los valores.
**
**Retorna en su nombre:
**	MEM_ERROR  - si no hay suficiente memoria.
**	OK	   - si no hubo problemas.
**
**Recibe:
**	max_number - M ximo n mero en una ficha.
 */
static char 
create_players(player_t *players, int max_number);


/*Crea el tablero e inicializa la estructura.
**
**Retorna en su nombre:
**	MEM_ERROR  - si no hay suficiente memoria.
**	OK	   - si no hubo problemas.
**
 */
static char
create_board(board_t *board);


/*Crea el tablero e inicializa la estructura.
**
**Retorna 
**	En su nombre:
**		MEM_ERROR  - si no hay suficiente memoria.
**		OK	   - si no hubo problemas.
 */
static char
create_board(board_t *board);


/*Toma una ficha del pozo y se la entrega a un jugador.
**
**Retorna:
**	En el par metro:
**		 chip		- Un puntero a la ficha.
**
**	En su nombre:
**		MEM_ERROR  	- Si se produjo un error de memoria.
**		EMPTY_POOL 	- Si no hay m s fichas en el pozo.
**		OK	   	- Si no hubo problemas.
**
**Recibe:
**	j - Sub  ndice del jugador al cu l se le va a entregar la ficha.
 */
static char
get_from_pool(gameADT game, player_index_t j, chip_t **chip);


/*Coloca la ficha de un jugador en el tablero.
**
**Retorna en su nombre:
**	MEM_ERROR   	- No hay memoria suficiente
**	END_OF_TURN 	- Una vez jugada una ficha el turno termina
**
**Recibe:
**	i		- Sub  ndice del jugador que va a colocar la ficha.
**	chip  		- Ficha del jugador a colocarse.
**	side		- Lado del tablero sobre el cual debe colocarse
**			  la ficha. Si es la primera debe recibir SIDES.
**
**OBS: La función debe recibir la ficha ordenada, o sea que si el par 
**ordenado es (x,y), entonces x < y.
**
**OBS: La funci n voltea la ficha si en la posici n recibida la misma
**no coincide con la del tablero, pero asume que la ficha realmente puede
**jugarse sobre el lado indicado con el par metro side.
 */
static char 
put_chip_on_board(gameADT game, chip_t *chip, side_index_t side,
  player_index_t i);


/*Calcula el valor de una ficha
**
**Retorna en su nombre:
**	El valor de la ficha.
**
**Recibe:
**	chip	- Ficha sobre la cual se desea saber el valor.
*/
static int
chip_value(chip_t chip);


/*Altera el orden de los n meros que componen al par ordenado que modela
**una ficha.
**
**Retorna en el par metro:
**	chip	- la ficha volteada. 
**
**Recibe:
**	chip	- Ficha que se desea voltear.
 */
static void 
invert_chip(chip_t *chip);


/*Eval a la posibilidad y conveniencia de jugar una ficha.
**
**Retorna en su nombre:
**  PLAY_RIGHT_GUARANTEED:	si jugando a la derecha me garantiza poder
**				jugar el siguiente turno.
**
**  PLAY_LEFT_GUARANTEED:	si jugando a la izquierda me garantiza poder
**				jugar el siguiente turno.
**
**  PLAY_LEFT_NOT_GUARANTEED:	si puedo jugarla a la izquierda, pero no me
**				garantiza que el siguiente turno pueda jugar.
**
**  PLAY_RIGHT_NOT_GUARANTEED:	si puedo jugarla a la derecha, pero no me 
**				garantiza que el siguiente turno pueda jugar.
**
**  NOTHING:			si no es posible jugarla
**
**Recibe:
**	chip		- Ficha a evaluar.
 */
static char 
evaluate(gameADT game, chip_t *chip);


/*Compara dos fichas.
**
**Retorna:
**  EQUAL	 		- si  chipA = chipB
**  A_HIGHER_THAN_B  	- si  chipA > chipB 
**  A_LOWER_THAN_B	- si  chipA < chipB.
**
**Recibe:
**	chipA ,chipB	- Fichas a evaluar
**
**Criterio:  Se compara el valor de los n meros de las fichas, considerando
**	     que X tiene mayor peso que Y por ejemplo (2,1) > (1,9)
 */
static int 
compare_chips(const chip_t *chipA, const chip_t *chipB);


/*Simplemente retorna 1, es utilizada para lograr una lista desordenada
**Para m s informaci n, ver list.h
 */
static int
out_of_order_list_compare_func (void *a, void*b);


/*Funci n de comparaci n para trabajar con listas ordenadas aceptando
**elementos repetidos.
**
**Retorna:
**  EQUAL 			- si  a=b
**  A_HIGHER_THAN_B  	- si  a>b 
**  A_LOWER_THAN_B	- si  a<b.
**  0				- si alguno de los dos es NULL (VER list.h).
**
**Recibe:
**	chipA ,chipB	- Fichas a evaluar
**
**Criterio : chipA > chipB  si el valor de chipA es mayor al valor de chipB
 */
static int 
ordered_list_compare_function(const chip_t *chipA, const chip_t *chipB);



/* ============================================================== */


/*Crea una estructura game_t e inicializa los valores
 */
gameADT 
create_game(int max_number, game_mode_t game_mode)
{
  gameADT game;

  game = (gameADT) malloc(sizeof(gameCDT));
  if(game != NULL){
    game->pc_counter_chips = (int *) calloc(sizeof(int), max_number + 1);
    if(game->pc_counter_chips == NULL)
      return NULL;
    game->counter_chips = (int *) calloc(sizeof(int), max_number + 1);
    if(game->counter_chips == NULL)
      return NULL;
    if(game->pc_counter_chips != NULL)	  
      if( (game->pool = create_pool(max_number)) != NULL )	
	if( (create_players(game->players, max_number)) == OK )
	  if( create_board(&game->board) != MEM_ERROR){
	    game->last_taked_chip = NULL; 
	    game->undo = CHIP_OUT;
	    game->max_number = max_number;
	    game->game_mode = game_mode;
	    game->turn = MAN;
	    return game;
	  }
  }
  return NULL;
}


/*Libera la memoria recervada al crear e juego
 */
void free_game(gameADT game)
{
  player_index_t i;

  for(i=MAN; i < NUMBER_OF_PLAYERS; i++)
    FreeList(game->players[i].chips);
  FreeList(game->board.chips);
  free_pool(game->pool);
  free(game);
}


/*Setea la penalizaci n al jugador del corriente turno, seg n el valor de 
**command. Los posibles valores de command son los mismos que el usuario 
**debe ingresar.
 */
void add_penalty(gameADT game, char command)
{
  switch(command){
    case 'p':
	      game->players[game->turn].penalty -= game->max_number / 2;
	      break;
    case 'd':
	      game->players[game->turn].penalty -= game->max_number;
	      break;	
  }
}


/*Retorna el valor de penalizaci n del jugador "i"*/
int penalty(gameADT game, player_index_t i)
{
  return game->players[i].penalty;
}


/*Retorna la cantidad de elementos en la lista "list"*/
int number_of_chips(gameADT game, list_index_t list)
{
  if(list == L_BOARD)
    return Size(game->board.chips);
  return Size(game->players[list].chips);
}


/*Informa el estado del pozo.
 */
char empty_pool(gameADT game)
{
  if(remaining_chips(game->pool))
    return FALSE;
  return TRUE;
}


/*Retorna el sub  ndice del jugador al cual le toca jugar
 */
player_index_t turn(gameADT game)
{
  return game->turn;
}


/*Setea el siguiente turno, inicializa lo pertinente al comando undo
**y retorna el sub  ndice del jugador al cual le toca jugar
 */
player_index_t next_turn (gameADT game)
{
  game->last_taked_chip = NULL;	
  return game->turn = (game->turn == MAN)?PC:MAN;
}


/*Retorna la suma del valor de todas las fichas en poseci n del jugador i*/
int total_value(gameADT game, player_index_t i)
{  
  chip_t *chip = (chip_t*) ! NULL;
  listADT list = game->players[i].chips;
  int total = 0;

  ToBegin(list);
  while((chip = NextElement(list)) != NULL)
    total += chip_value(*chip);

  return total;
}


/*Eval a si el juego termino.
 */
char check_game_over(gameADT game)
{
  chip_t *first_chip = game->board.left, *end_chip = game->board.right;

  if(Size(game->players[game->turn].chips) == 0)
    return WITHOUT_CHIPS;
  
  if(game->counter_chips[first_chip->x] > game->max_number + 1 && 
  game->counter_chips[end_chip->y] > game->max_number + 1)  /*¡¡ > y no == !!*/
    return TRANCA;
  return NOTHING;
}


/*Entrega una ficha perteneciente al Tablero o jugador en particulas segun
**indique el par metro list.
 */
void chips(gameADT game, list_index_t i, iterate_t command, 
    char numbers[NUMBERS_PER_CHIP])
{
  chip_t *chip;
  listADT list;

  if(i == L_BOARD){
    list = game->board.chips;
  }
  else{
    list = game->players[i].chips;
  }

  if(command == RESET){
    ToBegin(list);
  }
  
  chip = (chip_t *)NextElement(list);

  if(chip == NULL){
    numbers[LEFT] = END;
    numbers[RIGHT] = END;
  }
  else{
  numbers[LEFT] = chip->x;
  numbers[RIGHT] = chip->y;
  }
}


/*Juega una ficha del jugador persona en el tablero.
**
**1)Comprueba que la ficha enviada por el usuario y referida al tablero
**sea correcta.
**
**	Se aprobecha el hecho de que !X = Y y !Y = X. Ambas constantes indican
**	los numeros que componen el par ordenado que modela la ficha.
**
**2)Comprueba que la jugada sea valida.
**
**	Para ello verifico que alguno de los dos numeros del par ordenado que
**	modela la ficha del jugador coincida con el numero del extremo del 
**	tablero.
**
**3)"Aprobechando" que quedan cubiertos los casos:
**
**	Colocar al lado de la "ficha izquierda" del tablero.
**	Colocar al lado de la "ficha derecha" del tablero.
**	Colocar el lado de la "primer ficha" del tablero.
**
**se determina el "lado" sobre el cual hay que colocar la ficha.
**
**4)Finalmente si el jugador realmente tiene la ficha, se la coloca en el 
**  tablero.
 */      
  
char
play_chip(gameADT game, int p_chip[NUMBERS_PER_CHIP],
 int board_chip[NUMBERS_PER_CHIP])
{
  chip_t player_chip, *left_chip = game->board.left; 
  chip_t *right_chip = game->board.right;
  side_index_t side;
  player_t *player = &(game->players[game->turn]);
  int i;

  /*Acomodo la ficha referida al jugador, enviada por el usuario,
  **para poder usar listas ordenadas. 
  */
  side = RIGHT;
  if(p_chip[LEFT] > p_chip[RIGHT])
    side = LEFT;
  player_chip.y = p_chip[side];
  player_chip.x = p_chip[! side];
  
  /*Los primeros tres pasos descriptos anteriormente*/
  for(i = X, side = _NONE_SIDE; i < NUMBERS_PER_CHIP && side==_NONE_SIDE; i++){
    if(board_chip[i] == left_chip->x && board_chip[! i] == left_chip->y)
      if(player_chip.y == left_chip->x || player_chip.x == left_chip->x) 
	side = LEFT;
    if(board_chip[i] == right_chip->x && board_chip[! i] == right_chip->y)
      if(player_chip.x == right_chip->y || player_chip.y == right_chip->y)
	side = RIGHT;
  }
  
  if(side != _NONE_SIDE)
  {
    char comp = _A_HIGHER_THAN_B;
    chip_t *finder;

    /*El cuarto paso descripto anteriormente*/
    ToBegin(player->chips);
    while(finder != NULL && comp == _A_HIGHER_THAN_B){
      finder = (chip_t*)NextElement(player->chips);
      comp = ordered_list_compare_function(finder,&player_chip);
    }
    if(comp == _EQUAL){
      put_chip_on_board(game, finder, side, MAN);
      return END_OF_TURN;
    }
  }
  return SYNTAX_ERROR;
}


/*Toma una ficha del pozo y se la entrega al jugador del corriente turno
 */
char take_chip (gameADT game)
{
  chip_t *chip;
  switch(get_from_pool(game, game->turn, &chip)){
    case MEM_ERROR:
		    return MEM_ERROR;
    case EMPTY_POOL:
		    return EMPTY_POOL;
  }
  return OK;
}


/*Reparte ("max_number" + 1) fichas a cada jugador.
 */
char
deal_chips(gameADT game)
{
  int i;
  player_index_t j;
  chip_t *chip;	/*se usa solo para poder llamar a get_from_pool()*/

  int amount = game->max_number + 1;
  if(remaining_chips(game->pool) < amount * NUMBER_OF_PLAYERS)
    return EMPTY_POOL;

  for(j = MAN; j < NUMBER_OF_PLAYERS; j++){
    for(i = 0; i < amount; i++){
      if(get_from_pool(game, j, &chip) == MEM_ERROR)  
	return MEM_ERROR;
    }
  }
  return OK;
}


/*Funcion para el uso del comando undo:
**	a) Si el jugador saco una o m s fichas despu s de la  ltima invocaci n
**	   a la funci n en el corriente turno, entonces le quita la  ltima que
**	   saco.
**	b) Si no se cumple a) y si en la  ltima invocaci n a la funci n,
**	   durante el corriente turno, la misma quito una ficha al jugador,
**	   entonces se la devuelve.
**	c) Si no se tomaron fichas en el corriente turno, no hace nada.
**
**IMPORTANTE , Recordar: 
**      1) Seterar last_takerd_chip = NULl al pasar de turno.
**	2) Al sacar una ficha, antes de alterar game->last_taked_chip, si
**	   game->undo = CHIP_IN, llamar a la funci n seteando previamente
**	   game->undo = POOL, para devolver la ficha al pozo.
 */
char undo_command(gameADT game)
{
  player_t *player = &game->players[MAN];
  chip_t *last_taked_chip = game->last_taked_chip;

  if(last_taked_chip != NULL){
    if(game->undo == POOL){
	if(! put_chip(game->pool, *last_taked_chip))
	  return MEM_ERROR;
	game->undo = CHIP_OUT;
    }
    else if(game->undo == CHIP_OUT){
      Delete(player->chips, last_taked_chip);
      game->undo = CHIP_IN;
    }
    else if(game->undo == CHIP_IN){
      if(Insert(player->chips, last_taked_chip) == 0)
	return MEM_ERROR;
      game->undo= CHIP_OUT;
    }
  return TRUE;
  }
  return FALSE;
}


/*Estrategia de la computadora: 
**OBS: Al terminar NO setea el turno siguiente
*/
char pc_turn(gameADT game)
{
    int highest_side;
    player_t *pc = &game->players[PC];
    chip_t *best_chip[] = {NULL,NULL},
	  *highest_chip = NULL, *chip;
    int z,number_of_chips;
    char end_loop, state;

    /*Recorro la mano hasta encontrar una ficha, que me permita jugar el
    **siguiente turno (best_chip[SIDE]), o hasta llegar al final. 
    **En el mismo recorrido guardo adem s la primer ficha que se pueda jugar y 
    **sobre que lado lo puedo hacer. 
    **El hecho de que est n ordenadas por valor me garantiza que la primer
    **ficha encontrada sea la mayor posible en ambos casos.*/	
    
    number_of_chips = Size(pc->chips);
    ToBegin(pc->chips);
    highest_side = _NONE_SIDE;
    for(z = 0, end_loop = _LOOP; z < number_of_chips && end_loop == _LOOP; z++){
      chip =  (chip_t *)NextElement(pc->chips);
      switch(evaluate(game, chip)){
	  case _PLAY_LEFT_GUARANTEED:	
			best_chip[LEFT] = chip; 
			highest_side = LEFT;
			end_loop = ! _LOOP; 
			break;
	  case _PLAY_RIGHT_GUARANTEED:	
			best_chip[RIGHT] = chip; 
			highest_side = RIGHT;
			end_loop = ! _LOOP; 
			break;	  	      
	  case _PLAY_LEFT_NOT_GUARANTEED:
			if(highest_side == _NONE_SIDE ){
			  highest_side = LEFT;
			  highest_chip = chip;
			}
			break;
	  case _PLAY_RIGHT_NOT_GUARANTEED:
			if(highest_side == _NONE_SIDE ){
			  highest_side = RIGHT;
			  highest_chip = chip;
			}
			break;
	}
    }
    /*Coloco en best_chip[BEST] la ficha que voy a jugar*/
    if(best_chip[LEFT] == NULL){
      if(best_chip[RIGHT] != NULL)
	best_chip[_BEST] = best_chip[RIGHT];
      else if(highest_chip != NULL)
 	best_chip[_BEST] = highest_chip;
    }
    /*OBS: BEST = LEFT = 0*/
    if(best_chip[_BEST] != NULL)
      state = put_chip_on_board(game, best_chip[_BEST], highest_side, PC);
    else{   /*Si no juge, levanto hasta conseguir la primera que pueda jugar*/
      state = _LOOP;
      while(state == _LOOP){
	state = get_from_pool(game, PC, &chip);
	if (state == OK) /*Juego la primera que pueda jugar*/
	  switch(evaluate(game, chip)){
	    case _PLAY_LEFT_GUARANTEED:
	    case _PLAY_LEFT_NOT_GUARANTEED:
		  state = put_chip_on_board(game, chip, LEFT, PC);
		  break;
	    case _PLAY_RIGHT_GUARANTEED: 
	    case _PLAY_RIGHT_NOT_GUARANTEED:
		  state = put_chip_on_board(game, chip, RIGHT, PC);
		  break;
	    default:
		  state = _LOOP;
	  }
	}
    if(state == EMPTY_POOL)
      add_penalty(game, 'p');	/*Paso, si no puedo jugar*/
    }
    return state;
}


/*Juega la ficha m s alta entre los jugadores.
 */
char
play_highest_chip(gameADT game)
{
  player_index_t i;
  player_t *players = &(game->players[game->turn]);
  chip_t *chip[NUMBER_OF_PLAYERS];

  for(i = MAN; i < NUMBER_OF_PLAYERS ; i++){
    ToBegin(players[i].chips);
    chip[i] = (chip_t*)NextElement(players[i].chips);
  }
  i--;
  if(chip_value(*chip[MAN]) >= chip_value(*chip[PC]))
    i = MAN;
 
  game->turn = i;
  if(put_chip_on_board(game, chip[i], SIDES, i) == MEM_ERROR)
    return MEM_ERROR;
  return OK;
}


/*Si el jugador tiene el doble principal lo juega.
 */
char
play_double_main(gameADT game)
{
  chip_t main_chip, *chip;
  player_index_t i = game->turn;

  main_chip.x = main_chip.y = game->max_number;

  ToBegin(game->players[i].chips);
  if( ! compare_chips(chip = NextElement(game->players[i].chips), &main_chip) ){
    if(put_chip_on_board(game, chip, SIDES, i) == MEM_ERROR)
      return MEM_ERROR;
    return TRUE;
  }
  return FALSE;
}


/*Guarda los datos necesarios para poder retomar la partida luego 
**en el arhcivo filename.
 */
char
save(gameADT game, char* filename)
{
  int size;
  chip_t *chip;
  player_index_t i;
  FILE *saved_game;

  saved_game = fopen(filename, "wb");
  if(saved_game == NULL)
    return MEM_ERROR;

  fwrite(&game->game_mode,sizeof(int), 1, saved_game);
  fwrite(&game->turn + 1,sizeof(int), 1, saved_game);
  fwrite(&game->max_number,sizeof(int), 1, saved_game);

  for(i = MAN; i < NUMBER_OF_PLAYERS; i++){
    fwrite(&game->players[i].penalty, sizeof(int), 1, saved_game);
    size = Size(game->players[i].chips);
    fwrite(&size, sizeof(int), 1, saved_game);
    ToBegin(game->players[i].chips);
    while((chip = NextElement(game->players[i].chips)) != NULL){
      fwrite(&chip->x, sizeof(int), 1, saved_game);
      fwrite(&chip->y, sizeof(int), 1, saved_game);
    }
  }
	
  ToBegin(game->board.chips);
  while((chip = NextElement(game->board.chips)) != NULL ){
    fwrite(&chip->x, sizeof(int), 1, saved_game);
    fwrite(&chip->y, sizeof(int), 1, saved_game);
  }

  fclose(saved_game);

  return OK;	
}


/*Utilizada simplemente para evitar repetir c digo en la funci n load
 */
static gameADT 
free_aux(gameADT game)
{
  free_game(game);
  return NULL;
}


/*Carga del archivo filename los datos necesarios para retomar la partida
**guardada.
 */
gameADT
load(char* filename)
{
  int size, max_number, game_mode, chip_number[SIDES];
  gameADT game;
  side_index_t s;
  player_index_t i;
  FILE *saved_game;
  chip_t chip, *real_chip;

  saved_game = fopen(filename, "rb");
  if(saved_game == NULL)
    return NULL;
 
  /*Obtengo primero max_number y game_mode para crear el nuevo juego*/
  /*Modo de juego*/
  if(! fread(&game_mode, sizeof(int), 1, saved_game))
    return free_aux(game);
  /*M ximo n mero en una ficha*/
  if(fseek(saved_game, 2 * sizeof(int), SEEK_SET) == -1)
    return NULL;
  if(! fread(&max_number,sizeof(int), 1, saved_game))
    return NULL;
  if(max_number < 6 || max_number > 24)
    return NULL;

  /*Creo un nuevo juego*/
  game = create_game(max_number, game_mode);
  if(game == NULL)
    return NULL;

  /*Procedo a levantar el resto de la informaci n*/
  
  /*Turno*/
  if(fseek(saved_game, sizeof(int), SEEK_SET) == -1)
    return NULL;
  if(! fread(&game->turn,sizeof(int), 1, saved_game))
    return free_aux(game);
  if(game->turn != 2 && game->turn != 1)
    return free_aux(game);
  game->turn--;

  /*Salteo max_number porque ya lo le  */
  if(fseek(saved_game, sizeof(int), SEEK_CUR) == -1)
    return free_aux(game);
  
  /*Levanto la informaci n de los jugadores*/
  for(i = MAN; i < NUMBER_OF_PLAYERS; i++){

    /*Penalizaci n*/
    if(! fread(&game->players[i].penalty, sizeof(int), 1, saved_game))
      return free_aux(game);

    /*Cantidad de fichas*/
    if(! fread(&size, sizeof(int), 1,saved_game))
      return free_aux(game);
    if(remaining_chips(game->pool) < size)
      return free_aux(game);

    /*Obtengo las fichas de los jugadores*/
    for(s = RIGHT; size > 0; size--, s = RIGHT){	

      /*Levanto una ficha y si es necesario la volteo para poder ordenarla*/
      if(! fread(chip_number, sizeof(int), 2, saved_game))
        return free_aux(game);
      if(chip_number[LEFT] > chip_number[RIGHT])	
	s = LEFT;
      chip.y = chip_number[s];
      chip.x = chip_number[! s];

       /*Quito la ficha del nuevo pozo, verificando asi si la ficha es v lida,
       **y se la entrego a jugador.*/
      if((real_chip = get_specific_chip(game->pool, chip)) != NULL){
	Insert(game->players[i].chips, real_chip);
        /*Seteo los valores pertinentes a la estretegia de la m quina*/
	if(i == PC){
	  game->pc_counter_chips[chip.x] ++;
	  game->pc_counter_chips[chip.y] ++;
	}
      }
      else
	return free_aux(game);

    }  
  }
  
  /*Levanto las fichas del tablero*/
  while(fread(chip_number, sizeof(int), 2, saved_game)){
    chip.x = chip_number[LEFT];
    chip.y = chip_number[RIGHT];
    /*Quito la ficha del nuevo pozo, verificando asi si la ficha es v lida,
    **y la coloco en el tablero.*/
    real_chip = get_specific_chip(game->pool, chip);
    if(real_chip->x != chip.x)	/*Acomodo la ficha de pozo de ser necesario*/
      invert_chip(real_chip);
    EndInsert(game->board.chips, real_chip);
    game->counter_chips[chip.x]++;	  /*Necesario para evaluar tranca*/
    game->counter_chips[chip.y]++; 
  }
  /*Compruebo que no haya salido del ciclo por un error*/
  if(! feof(saved_game))
      return free_aux(game);
  /*Inicializo los valores de la estructura board_t*/
  ToBegin(game->board.chips);
  game->board.left = (chip_t*)NextElement(game->board.chips);
  ToEnd(game->board.chips);
  game->board.right = (chip_t*)NextElement(game->board.chips);
      
  fclose(saved_game);

  return game;	
}


/*Crea las manos de los jugadors e inicializa los valores.
 */
static char 
create_players(player_t *players, int max_number)
{
  int i;
  
  for(i=0; i < NUMBER_OF_PLAYERS; i++){
    players[i].penalty = 0;
    players[i].chips = NewList((int (*) (void*, void*))
					ordered_list_compare_function);
    if(players[i].chips == NULL && i == PC){	
      FreeList(players[MAN].chips);
      return MEM_ERROR;
    }
  }
  return OK;
}



/*Crea el tablero e inicializa la estructura.
 */
static char
create_board(board_t *board)
{
  board->chips = NewList(out_of_order_list_compare_func);
  if(board == NULL)
    return MEM_ERROR;
  board->left = NULL;
  board->right = NULL;
  return OK;
}


/*Toma una ficha del pozo y se la entrega al jugador cuyo sub  ndice es j.
**Adem s, en el par metro chip_t *chip, devuelve un puntero a la misma.
 */
static char
get_from_pool(gameADT game, player_index_t j, chip_t **chip)
{
  player_t *player = &game->players[j];
 
  if( (*chip = get_chip(game->pool)) == NULL)
    return EMPTY_POOL;

  /*Seteo los valores pertinenetes a cada jugador*/
  switch(j){
    case MAN:	/*Pertinente al comando undo*/
	      if(game->undo == CHIP_IN){
		game->undo = POOL;
		undo_command(game);
	      }
	      game->last_taked_chip = *chip;
	      break;
    case PC:	/*Pertinente a pc_turn()*/
	      game->pc_counter_chips[(*chip)->x] ++;
	      game->pc_counter_chips[(*chip)->y] ++;
	      break;
  }
  if(Insert(player->chips, *chip) == 0)
    return MEM_ERROR;

  return OK;
}


/*Coloca la ficha chip del jugador "i" sobre el lado "side" del tablero.
 */
char put_chip_on_board(gameADT game, chip_t *chip, side_index_t side,
  player_index_t i)
{
  char aux;
    
  /*La ficha debe estar en la correcta posici n (x,y) con x < y*/
  Delete(game->players[i].chips, chip);
  
  game->counter_chips[chip->x]++;
  game->counter_chips[chip->y]++;

  if(side == LEFT){	/*Acomodo la ficha y la coloco a la izquirda*/
    if(game->board.left != NULL )	
      if(chip->y != game->board.left->x)
	invert_chip(chip);
    aux = BeginInsert(game->board.chips, chip);
    game->board.left = chip;
  }
  else if(side == RIGHT){ /*Acomodo la ficha y la coloco a la derecha*/
    if(game->board.right != NULL)
      if(chip->x != game->board.right->y)
	invert_chip(chip);
    aux = EndInsert(game->board.chips, chip);
    game->board.right = chip;
  }
  else{
    aux = BeginInsert(game->board.chips, chip);
    game->board.right = chip;
    game->board.left = chip;
  }
  
  if(aux == 0)
    return MEM_ERROR;

  if(i == PC){	/*Pertinente a para pc_turn()*/
    game->pc_counter_chips[chip->x]--;
    game->pc_counter_chips[chip->y]--;
  }
  return END_OF_TURN;
}


/*Retorna el valor de la ficha*/
static 
int chip_value(chip_t chip)
{
  return chip.x + chip.y;
}


/*Voltea una ficha.
**Recibe (x,y) y retorn (y,x)*/
static void invert_chip(chip_t *chip)
{
  int aux;
  aux=chip->x;
  chip->x=chip->y;
  chip->y=aux;
}


/*Evalúa la posibilidad y conveniencia de jugar una ficha.
 */
static char evaluate(gameADT game, chip_t *chip)
{
  char end_loop = 0;
  int *pc_counter_chips = game->pc_counter_chips;
  side_index_t i;

  pc_counter_chips[chip->x]--;	/*Me indica cuantos sies, cincos, etc, tengo*/
  pc_counter_chips[chip->y]--;	/*quito los que quiero evaluar*/
  i=LEFT;

  if(chip->x == game->board.left->x || chip->y == game->board.left->x){

  /*Compruebo si al jugar la ficha me quedan numeros que me pemitan jugar 
  **en ambos lados, y por lo tanto me aceguren jugar el próximo turno*/

  if(pc_counter_chips[chip->x] > 0 && 
  pc_counter_chips[game->board.right->y] > 0)
    end_loop = _PLAY_LEFT_GUARANTEED;
  else
      end_loop = _PLAY_LEFT_NOT_GUARANTEED;
  }
  if(end_loop != 1){  /*Si no pude asegurarme de jugar el pr ximo turno*/
    if(chip->y == game->board.right->y || chip->x == game->board.right->y){
      if(pc_counter_chips[chip->y] > 0 && 
      pc_counter_chips[game->board.left->x] > 0)
	end_loop = _PLAY_RIGHT_GUARANTEED;
      else 
	end_loop = _PLAY_RIGHT_NOT_GUARANTEED;
    }
  }

  pc_counter_chips[chip->x]++;	/*Se lo sumo, porque el que toma a decici n*/
  pc_counter_chips[chip->y]++; 	/*final de colocar la ficha es el que invoca*/
  return end_loop;
}


/*Compara dos fichas.
**
**Criterio:  Se compara el valor de los n meros de las fichas. Se considera
**	     que X tiene mayor peso que Y por ejemplo (2,1) > (1,9)
 */
static int 
compare_chips(const chip_t *chipA, const chip_t *chipB)
{
    if( chipA->x > chipB->x )
      return _A_HIGHER_THAN_B;
    if(chipA->x == chipB->x){
      if(chipA->y == chipB->y)
	return _EQUAL;
      if(chipA->y > chipB->y)
	return _A_HIGHER_THAN_B;
    }
    return _A_LOWER_THAN_B;
}


/*Simplemente retorna 1, es utilizada para lograr una lista desordenada
 */
static int
out_of_order_list_compare_func (void *a, void*b)
{
  return 1;
}


/*Funci n de comparaci n para tabajar con listas ordenadas acenpando
**elementos repetidos.
**
**Creterio : chipA > chipB  si el valor de chipA es mayor al valor de chipB
 */
static int 
ordered_list_compare_function(const chip_t *chipA, const chip_t *chipB)
{
 int a,b; 
  if(chipA != NULL && chipB !=NULL){
    if( (a = chip_value(*chipA)) > (b = chip_value(*chipB)) )
      return _A_HIGHER_THAN_B;
    if(a < b)
      return _A_LOWER_THAN_B;
    else
      return compare_chips(chipA, chipB);  /*Val(A)=Val(B) no impica A=B*/
  }					   
  return _EQUAL;
}


