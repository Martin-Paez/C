#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "dominoBack.h"
#include "getnum.h"


/*Equivale al asqui de la tecla enter*/
#define ENTER			10

/*Convierte un valor char a entero*/
#define TO_INT(x)		((x)-'0')

/*Identifican el men√∫ principal y el de selecci√≥n de modo de juego*/
#define FIRST_MENU_OPTIONS	3
#define SECOND_MENU_OPTIONS	3

/*Cantidad de letras para los comnados del usuario*/
#define FILE_LETTERS		20

/*Cantidad de letras para el nombre del archivo utilizado para
**guardar la partida.*/
#define COMMAND_LETTERS		(5 + FILE_LETTERS)

/*Utilizadas para implementar player_turn*/
#define AMOUNT_OF_CHIPS		2
#define START_TURN		0

/*M√°ximo y m√≠nimo numero que una ficha puede tener*/
#define MAX_NUMBER		25
#define MIN_NUMBER		6


typedef struct line_t{
  int 	amount;		/*Cantidad de caracteres*/
  char  *text;
}line_t;

typedef struct menu_t{
  int	amount;		/*Cantidad de opciones*/
  char  **options;	/*Texto para cada opcion*/
}menu_t;
    
/*Opciones del men√∫ principal*/
enum {START = 1, LOAD, EXIT};



/*Pide al usuario los datos necesarios para comenzar, crea el pozo ,los
 *jugadores y llama a la funci√≥n encargada de comenzar el juego
 */
void 
init_game(void);

  
/*Reparte las fichas a todos los jugadores y llama a la funcion que establece 
**la forma de colocar la primer ficha segun el contenido de "game_mode"
**
**Recive:
**	game_mode_t - Modo de juego.
 */
void 
start_game(gameADT game, game_mode_t game_mode);


/*Determina a quien le toca jugar, chequea el final del juego, e imprime
**el tablero, las fichas del jugador y los puntajes.
 */
void 
play_game(gameADT game);


/*Imprime los puntajes finales y al ganador.
**Recive:
**	TRANCA		- Si no hay m√°s jugadas posibles.
**	WITHOUT_CHIPS	- Si el jugador del corriente turno se quedo sin
**			  fichas.
 */
void 
game_over(gameADT, game_over_t);


/*Analiza que los comandos ingresados por el usuario sean v√°lidos y llama 
**a las funciones encargadas de llevar a cabo cada tarea.
 */
char 
player_turn (gameADT game);


/*Comunica al usuario lo que ocurre durante el comienzo del juego si se a
**seleccionado el modo "Primero el doble principal".
 */
void 
first_double_main(gameADT game);


/*Comunica al usuario lo que ocurre durante el comienzo del juego si se a
**seleccionado el modo "Siempre ficha m√°s alta".
 */
void 
always_highest_chip(gameADT game);


/*Interact√∫a con el usuario hasta que alguno de los dos jugadores juegue
**el doble principal.
 */
void 
always_double_main(gameADT game);


/*Juega el doble principal si es que algun jugador lo tiene.
**Retorna:
**	TRUE  - Si pudo jugarse el doble principal.
**	FALSE - Si ninguno de los jugadores tiene el doble principal.
 */
char 
check_double_main(gameADT game);


/*Seg√∫n se indique con el par√°metro list , imprime las fichas del:
**Recive:
**	Tablero   - list = L_BOARD
**	Jugador   - list = L_MAN
**	Ordenador - list = L_PC
 */
void 
print_chips(gameADT game, list_index_t list);


/*Llama a las funciones encargadas de la impresi√≥nn del estado de juego
 */
void 
print_info(gameADT game);


/*Imprime la informaci√≥n del estado de juego referiada a los jugadores
 */
void 
print_players(gameADT game);


/*Imprime un menu con los par√°metro establecidos en una estructura menu_t,
**y retorna la eleccion de usuario
 */  
char 
menu(menu_t menu);


/*Lee "line.amount" caracteres del buffer de entrada, los guarda en
**line.text y luego limpia el bufer.
 */
void 
get_line(line_t line);


/*Intercambia los elementos*/
void 
swap(int, int);


/*Libera la memoria y sale del programa*/
void 
quit(gameADT game);


void 
mem_error(gameADT game);


/*Guarda los datos necesarios para poder retomar la partida luego 
**en el arhcivo filename.
**Retorna:
**	MEM_ERROR	- Si no pudo crear el archivo
**	OK			- Si no hubo ningun problema
 */
char
save_game(gameADT, char *filename);


/*Carga del archivo filename los datos necesarios para retomar la partida 
**guardada.
**Retorna:
**	ERROR		- Si no pudo abrir el archivo
**	OK			- Si no hubo ningun problema
 */
void
load_game(void);


/*========================================================*/


/*Menu principal
 */
int main (void)
{
  char *options[] = {"1)Comenzar juego nuevo","2)Recuperar juego guardado",
      "3)Salir"};
  menu_t first_menu;

  first_menu.amount = FIRST_MENU_OPTIONS;
  first_menu.options = options;
  while(1){
    switch(menu(first_menu)){
      case START: init_game();	  break;
      case LOAD:  load_game();	  break;
      case EXIT:  exit(0);
    }
  }
}


/*Pide al usuario los datos necesarios para comenzar, crea el pozo ,los
 *jugadores y llama a la funci√≥n encargada de comenzar el juego
 */
void init_game(void)
{ 
  char *options[] = {"1)Primero doble principal","2)Siempre ficha m·s alta",
      "3)Siempre doble principal"};
  menu_t second_menu;
  int max_number = MAX_NUMBER + 1;
  gameADT game;
  game_mode_t game_mode;
  
  second_menu.amount = SECOND_MENU_OPTIONS;
  second_menu.options = options;
  game_mode = menu(second_menu);
  while(max_number<MIN_NUMBER || max_number>MAX_NUMBER)
    max_number = getint("%cM·ximo n˙mero en una ficha (6-24): ", ENTER);

  if( (game = create_game(max_number, game_mode)) != NULL )
    start_game(game, game_mode);  
  else{
    printf("No hay suficiente memoria%c", ENTER);
    exit(1);
  }
}

  
/*Reparte las fichas a todos los jugadores y llama a la funcion que establece 
 *la forma de colocar la primer ficha segun el contenido de "game_mode"
 */
void start_game(gameADT game, game_mode_t game_mode)
{
  printf("%c* ====================================== *%c", ENTER, ENTER);
  printf("%cSe reparten las fichas:%c", ENTER, ENTER);
  if(!deal_chips(game)){
    printf("%cNo hay suficientes fichas",ENTER); 
    return;
  }
  print_players(game);
  switch(game_mode){
    case FIRST_DOUBLE: 		
			printf("%c%cOpciÛn: Primero el doble principal%c", 
			      ENTER, ENTER, ENTER);
			first_double_main(game);
			break;
    case ALWAYS_HIGHEST:
			printf("%c%cOpciÛn: Siempre la ficha m·s alta%c",
			      ENTER, ENTER, ENTER);
			always_highest_chip(game);
			break;
    case ALWAYS_DOUBLE: 
			printf("%c%cOpciÛn: Siempre el doble principal%c",
				ENTER, ENTER, ENTER);
			always_double_main(game);
			break;
  }
  play_game(game);
}


/*Determina a quien le toca jugar, chequea el final del juego, e imprime
**el tablero, las fichas del jugador y los puntajes.
 */
void play_game(gameADT game)
{
  char fin = 0, (*play_turn[])(gameADT) = {player_turn, pc_turn};
  player_index_t i;
  print_info(game);

  while(fin == 0){
    i = turn(game);
    printf("%cTurno del %s%c", ENTER, (i == MAN)?"Jugador":"ordenador", ENTER);
    switch (play_turn[i](game)){
      case EMPTY_POOL:	
			printf("Ya no quedan fichas en el pozo%c", ENTER);
			break;
      case MEM_ERROR:	
			mem_error(game);	
    }
    print_info(game);
    switch(check_game_over(game)){
      case WITHOUT_CHIPS:	 
			  game_over(game, WITHOUT_CHIPS);
			  fin = 1;
			  break;
      case TRANCA:	  
			  game_over(game, TRANCA);
			  fin = 1;
			  break;
      default:			 
			  next_turn(game);
    }
  }
}


/*Imprime los puntajes finales y al ganador
 */
void game_over(gameADT game, game_over_t result)
{
  int t, p, score[NUMBER_OF_PLAYERS];
  player_index_t i;

  if(result == WITHOUT_CHIPS)
    printf("%cFIN DEL JUEGO. El %s no tiene m·s fichas%c", ENTER,
      (turn(game) == MAN)?"JUGADOR":"ORDENADOR", ENTER);
  else
    printf("%cYa no hay jugadas posibles%c", ENTER, ENTER);
    
  print_players(game);
  putchar(ENTER);
  for(i = MAN; i < NUMBER_OF_PLAYERS; i++){
    t = total_value(game, !i);
    p = penalty(game, i);
    score[i] = t+p;
    printf("Puntaje del %s = %d - %d = %d puntos%c", 
      (i == MAN)?"Jugador":"Ordenador", t, p, score[i] , ENTER);
  }
  printf("%c%cGANO EL %s%c%c", ENTER, ENTER, 
    (score[PC] > score[MAN])?"ORDENADOR":"JUGADOR", ENTER, ENTER);
  free_game(game);
}



/*Analiza que los comandos ingresados por el usuario sean v√°lidos y llama 
**a las funciones encargadas de llevar a cabo cada tarea.
**OBS: Al terminar NO setea el turno siguiente
 */
char player_turn (gameADT game)
{
  char end_of_turn = START_TURN,  aux2[2], undo = FALSE;
  char s_command[COMMAND_LETTERS], s_file[FILE_LETTERS];
  int board_chip[NUMBERS_PER_CHIP], player_chip[NUMBERS_PER_CHIP], aux;
  line_t line;

  line.amount = COMMAND_LETTERS;
  line.text = s_command;  
  while(end_of_turn == START_TURN)
  {
    putchar('>');
    get_line(line);
    switch( s_command[0] ){
    case 'c': 
	      aux= sscanf(s_command, "c(%d,%d)(%d,%d%c",&player_chip[LEFT],
 		&player_chip[RIGHT], &board_chip[LEFT], &board_chip[RIGHT], aux2);
	      
	      if( aux == 5 && aux2[0] == ')'){
		end_of_turn = play_chip(game, player_chip, board_chip);
	      }
	      else
		end_of_turn = SYNTAX_ERROR;
	      break;

    case 's':	

	      if(! s_command[1]){	/*Verifico si solo es una letra*/
		if(undo == FALSE){
		    switch (take_chip(game)){
		      case EMPTY_POOL:	end_of_turn = EMPTY_POOL;  break;
		      case MEM_ERROR:	end_of_turn = MEM_ERROR;   break;
		    }
		}
		else{
		    undo_command(game);
		    undo = FALSE;
		  }
		  print_info(game);
		  putchar(ENTER);
	      }
	      else if(!strncmp(s_command,"save ", 5)){
		  sscanf(s_command + 5, "%s", s_file);
		  save_game(game,s_file);
		}
	      else
		  end_of_turn = SYNTAX_ERROR;

	      break;

    case 'u': 
	      if(!strcmp(s_command,"undo")){
		if(undo == FALSE){
		  if(undo_command(game) == TRUE)
		    undo = TRUE;
		  print_info(game);
		  putchar(ENTER);
		}
	      }
     	      else
		end_of_turn = SYNTAX_ERROR;
	      break;

    case 'p': 
	      printf("El jugador a pasado el turno sin jugar fichas%c", ENTER);
	      end_of_turn = END_OF_TURN;
	      add_penalty(game, s_command[0]);
	      break;

    case 'd': 
	      printf("Mano del Ordenador:%c", ENTER);
	      print_chips(game, L_PC);
	      putchar(ENTER);
	      add_penalty(game, s_command[0]);
	      end_of_turn = END_OF_TURN;
	      break;

    case 'q':
	      if(!strncmp(s_command,"quit", 4))
		quit(game);

    default: 
	      end_of_turn = SYNTAX_ERROR; 
	      break;

    }	

    switch(end_of_turn){
      case SYNTAX_ERROR:
			printf("InstrucciÛn no v·lida%c",ENTER);
			end_of_turn = START_TURN;
			break;
      case EMPTY_POOL:
			printf("Ya no quedan fichas en el pozo%c", ENTER);
			end_of_turn = START_TURN;
			break;
    }

  }
  return end_of_turn;
}


/*Comunica al usuario lo que ocurre durante el comienzo del juego si se a
**seleccionado el modo "Primero el doble principal".
 */
void first_double_main(gameADT game)
{
  player_index_t i;
  
  for(i = MAN; i < NUMBER_OF_PLAYERS && !play_double_main(game); i++)
    next_turn(game);
  if(i < NUMBER_OF_PLAYERS){
    printf("%cEl %s jugÛ doble principal%c", ENTER, 
	  (turn(game) == MAN)?"jugador":"ordenador", ENTER);
   next_turn(game);
  }
  else{
    printf("%cNingun jugador tiene el doble principal. Se jugar· la"
	   " ficha de mayor valor%c", ENTER, ENTER);
  always_highest_chip(game);
  }
}


/*Comunica al usuario lo que ocurre durante el comienzo del juego si se a
**seleccionado el modo "Siempre ficha m√°s alta".
 */
void always_highest_chip(gameADT game)
{
  if(play_highest_chip(game) == MEM_ERROR)
    quit(game);
  printf("%cEl %s jugÛ la ficha de mayor valor%c", ENTER, 
	(turn(game) == MAN)?"jugador":"ordenador", ENTER);
  next_turn(game);
}


/*Interact√∫a con el usuario hasta que alguno de los dos jugadores juegue
**el doble principal
 */
void 
always_double_main(gameADT game)
{
  char pool_state, i = 0;
  
  while(1){
    if(i++ >= NUMBER_OF_PLAYERS){
      printf("%c%s que levantar una ficha%c", ENTER, 
	    (turn(game) == MAN)?"Tenes":"Tengo", ENTER);
      pool_state = take_chip(game);
      printf("%c* ---------------------------------------- *", ENTER);
      print_players(game);
      printf("%c* ---------------------------------------- *", ENTER);
      putchar(ENTER);
      switch(pool_state){
	case MEM_ERROR:
			quit(game);
	case EMPTY_POOL:
			printf("%cEvidentemente me mentiste, no hay m·s fichas."
			      "Tenes que jugar el doble principal%c", ENTER,
				ENTER);
			if(turn(game) == PC) next_turn(game);
			play_double_main(game);
			next_turn(game);
			return;
			break;
      }
    }
    if(check_double_main(game)){
      printf("%c%cEl %s a colocado el doble principal%c", ENTER, ENTER,
	    (turn(game) == MAN)?"Jugador":"Ordenador", ENTER);
      next_turn(game);
      return;
    }
    next_turn(game);
  }
}


/*Juega el doble principal si es que algun jugador lo tiene.
 */
char check_double_main(gameADT game)
{
  switch(turn(game)){
      case MAN:  
		if(yesNo("%c¬øPodes jugar el doble principal?[S/N] ", ENTER)){
		  if(play_double_main(game))	
		    return TRUE;
		  else
		    printf("%cTe equivocaste, no tenes el doble principal%c",
			  ENTER, ENTER);
		}	
		break;
      
      case PC:	/*Se aprobecha que la funci√≥n es de orden 1*/
		if(play_double_main(game))	
		  return TRUE;
		printf("%cYo tampoco tengo el doble principal%c", ENTER,
			ENTER);
		break;
  }
  return FALSE;
}


/*Llama a las funciones encargadas de la impresi√≥nn del estado de juego
 */
void print_info(gameADT game)
{
  printf("%c* ---------------------------------------- *%c", ENTER, ENTER);
  printf("Estado de Juego:%c", ENTER);
  putchar(ENTER);
  printf("Tablero:%c",ENTER);
  print_chips(game, L_BOARD);
  putchar(ENTER);
  print_players(game);
  printf("%c* ---------------------------------------- *%c", ENTER, ENTER);
}


/*Imprime la informaci√≥n del estado de juego referiada a los jugadores
 */
void print_players(gameADT game)
{
  int amount;
  player_index_t z;
  /*OBS: PC = L_PC y MAN = L_MAN*/
  for(z = PC; z == PC || z==MAN; z--){
    printf("%c%s: %d Fichas - %d PenalizaciÛn ", ENTER,
      (z == MAN)?"Jugador":"Ordenador", amount=number_of_chips(game, z), 
      penalty(game, z));
    if(z == MAN)
      print_chips(game, z);	
  }
}


/*Seg√∫n se indique con el par√°metro list , imprime las fichas del:
**Recive:
**	Tablero   - list = L_BOARD
**	Jugador   - list = L_MAN
**	Ordenador - list = L_PC
 */
void print_chips(gameADT game, list_index_t list)
{
  int i;
  char chip[NUMBERS_PER_CHIP] = {0,0};
  
  for(i=0; i<number_of_chips(game, list) && chip[0] != END; i++){
    chips(game, list, i != RESET, chip);
    if(chip[0] != END)
      printf("(%d,%d)",chip[LEFT],chip[RIGHT]);
  }
}


/*Guarda los datos necesarios para poder retomar la partida luego 
**en el arhcivo filename.
**Retorna:
**	MEM_ERROR	- Si no pudo crear el archivo
**	OK		- Si no hubo ningun problema
 */
char
save_game(gameADT game, char *filename)
{
  FILE *saved_game;
  char flag = TRUE;
  
  
  if((saved_game = fopen(filename, "rt")) != NULL){
    flag = FALSE;
    if(yesNo("%cEl archivo existe desea sobreescribirlo?[S/N] ",ENTER))
      flag = TRUE;
    fclose(saved_game);
  }
  if(flag == TRUE){
    if(save(game, filename) == MEM_ERROR){
	printf("%cNo se pudo guardar el juego%c", ENTER, ENTER);
	return MEM_ERROR;
    }
    else
	printf("%cJuego Salvado%c", ENTER, ENTER);
  }
  return OK;
} 



/*Carga del archivo filename los datos necesarios para retomar la partida 
**guardada.
 */
void
load_game(void)
{
  char filename[FILE_LETTERS];
  gameADT game;
  line_t line;
  
  line.amount = FILE_LETTERS;
  line.text = filename;
  printf("Ingrese el nombre de la partida que desea cargar:%c", ENTER);
  get_line(line);
  game = load(filename);
  if(game != NULL){
    printf("%cJuego Cargado%c", ENTER, ENTER);	
    play_game(game);
  }
  else
  printf("%cNo se pudo cargar el archivo%c", ENTER, ENTER);
}


/*Imprime un menu con las establecidas en options[],y retorna la eleccion de usuario
 */  
char menu(menu_t menu)
{
  int i;
  int aux = menu.amount+1;
  if( menu.amount <= 0)
    return 0;
  for(i=0; i < menu.amount; i++)
    printf("%c%s", ENTER, menu.options[i]);
  while(aux < 1 || aux > menu.amount)
    aux = getint("%c%cElija una opciÛn (ingresar n˙mero): ", ENTER, ENTER);
  return aux;
}


/*Lee "line.amount" caracteres del buffer de entrada, los guarda en
**line.text y luego limpia el bufer.
 */
void get_line(line_t line)
{
  int i = 0;
  char c;
  
  while((c = getchar()) != ENTER){
    if(i < line.amount)
      line.text[i++] = c;
  }
  line.text[i] = 0;
}


/*Avisa al usuario de un error de memoria y llama a quit para 
**terminar el programa.
 */
void mem_error(gameADT game)
{
  printf("No hay memoria suficiente para continuar el juego");
  quit(game);
}


/*Intercambia el contenido de dos variables
 */
void swap(int a, int b)
{
  int aux;
  aux = b;
  b = a;
  a = b; 
}


/*Libera la memoria reservada para gameADT, y termina el programa
 */
void quit(gameADT game)
{
  free_game(game);
  exit(0);
}
