#ifndef _DOMINO_BACK_H_
#define _DOMINO_BACK_H_



#define NUMBER_OF_PLAYERS 	 2

/*Posibles retornos de función*/
#define OK			 	 1
#define TRUE			 1
#define FALSE			 0
#define NOTHING			 5
#define EMPTY_POOL		16
#define END_OF_TURN		15
#define ERROR 			-2
#define MEM_ERROR			-3
#define SYNTAX_ERROR		-4

/*Cantidad de números por cada ficha (izquierdo y derecho)*/
#define NUMBERS_PER_CHIP	 2

/*Número derecho o izquierdo de una ficha que JAMÁS será usado.
**Se utiliza para poder iterar sobre las fichas
 */
#define END			-5


/*Modos de juego*/
typedef enum {FIRST_DOUBLE = 1, ALWAYS_HIGHEST, ALWAYS_DOUBLE} game_mode_t;

/*Hacen referencia a las fichas en los extremos del tablero*/
typedef  enum {LEFT, RIGHT, SIDES} side_index_t;

/*Estructura del juego*/
typedef struct gameCDT *gameADT;

/*Hacen referencia a los jugadores*/
typedef enum {MAN, PC} player_index_t;

/*Hacen referencia a las manos de los jugadores y al tablero*/
typedef enum {L_MAN, L_PC, L_BOARD} list_index_t;

/*Para el uso de la función  game_over()*/
typedef enum {WITHOUT_CHIPS = 12, TRANCA}game_over_t;

/*Para el uso de la función chips()
**OBS: NEXT_CHIP, puede ser cualquier número entero distinto de RESET
 */
typedef enum{RESET, NEXT_CHIP}iterate_t;



/*==================================================================== */

/*Crea una estructura game_t e inicializa los valores
**
**Retorna:
**	NULL - Si se produjo algún error.
**
**Recibe:
**	max_number - Máximo número en una ficha.
**	game_mode  - Mode de juego.
*/
gameADT 
create_game(int max_number, game_mode_t game_mode);


/*Libera la memoria reservada al crear e juego
 */
void 
free_game(gameADT game);


/*Setéa la penalización al jugador del corriente turno, según el comando
**por el cual se lo va a penalizar.
**
**Recibe:
**   command - Posbles valores:
**
**	'p'(asqui de la letra p)  -  Suma la mitad del valor del nÃºmero 
**				     mÃ¡ximo en una ficha
**	'd'(asqui de la letra d)  -  Suma el valor del nÃºmero mÃ¡ximo en 
**				     una ficha.
 */
void 
add_penalty(gameADT game, char command);


/*Retorna el valor de penalización de un jugador
**
**Retorna en su nombre:
**	Penalización de un jugador.
**
**Recibe:
**	i - Subí­ndice del jugador sobre el cual se consulta la penalización.
 */
int 
penalty(gameADT game, player_index_t i);


/*Retorna la cantidad de fichas del tablero ó de un jugador en particular según indique
**el parámetro list.
**
**Retorna en su nombre:
**	La cantidad de fichas.
**
**Recibe:
**	list - Posibles valores:
**		L_BOARD - Tablero.
**		L_MAN	- Jugador.
**		L_PC	- Ordenador.
 */
int 
number_of_chips(gameADT game, list_index_t list);


/*Informa el estado del pozo (Lleno o vacío).
**
**Retorna:
**	TRUE  - Si el pozo está vacío
**	FALSE - Si todavía hay fichas en el pozo 0
 */
char 
empty_pool(gameADT game);


/*Indica a quien le toca jugar.
**
**Retorna:
**	 El subí­ndice del jugador en cuestión.
 */
player_index_t 
turn(gameADT game);


/*Setea el siguiente turno, inicializa lo pertinente al comando undo
**
**Retorna:
**	El subíndice del jugador al cual le toca jugar.
 */
player_index_t
next_turn (gameADT game);


/*Suma el valore de las fichas de un jugador.
**
**Retorna:
**	La suma del valor de todas las fichas en posesión del jugador.
**
**Recibe:
**	i - Subí­ndice del jugador sobre el cuál se consulta.
 */
int 
total_value(gameADT game, player_index_t i);


/*Evalúa si el juego termino.
**
**Retorna:
**	WITHOUT_CHIPS   - si el jugador del corriente no tiene mÃ¡s fichas
** 	TRANCA	  	- si no hay mÃ¡s jugadas posibles
** 	NOTHING         - si no se cumplen las anteriores
 */
char 
check_game_over(gameADT game);


/*Entrega una ficha perteneciente al Tablero o jugador en particular según
**indique el parámetro list.
**
**Retorna:
**	en numbers[], el par ordenado referente a una ficha.
**
**Recive:
**	list - Posibles valores:
**		Tablero   - si recive list = L_BOARD.
**		Jugador   - si recive list = L_MAN.
**		Ordenador - Si recive list = L_PC.
**
**OBS: La invocación a la función con el parámetro command = RESET entrega 
**en numbers el par ordenado que forma la ficha más alta que tenga el jugador
**o que se encuentre en el tablero (dependiendo de list_index_t i). A partir
**de la primer invocación del modo descrito, las siguientes invocaciones se 
**deberían realizar con el parámetro command = NEXT_CHIP, de forma tal de poder 
**iterar hasta que se reciba el par (END,END) el cual indica que se a recorrido
**toda la mano. Una vez hecho esto para volver a usar la función habrá que  
**invocarla como si fuese por primera vez.
**
**OBS: NEXT_CHIP, puede ser cualquier número entero distinto de RESET
 */
void 
chips(gameADT game, list_index_t list, iterate_t command, 
    char numbers[NUMBERS_PER_CHIP]);


/*Juega una ficha del jugador persona en el tablero.
**
**Retorna en su nombre:
**	SYNTAX_ERROR  - si las fichas no son válidas.
**	END_OF_TURN   - si no hubo problemas.
**
**Recibe:
**	player_chip - Par ordenado que modela la ficha del jugador.
**	board_chip  - Par ordenado que modela la ficha del tablero.
**
**OBS: Se considera que el tablero tiene al menos una ficha.
 */
char
play_chip(gameADT game, int player_chip[NUMBERS_PER_CHIP], 
    int board_chip[NUMBERS_PER_CHIP]);


/*Toma una ficha del pozo y se la entrega al jugador de corriente turno
**
**Retorna en su nombre:
**	MEM_ERROR	- Si se produjo un error de memoria.
**	EMPTY_POOL 	- Si no hay más fichas en el pozo.
**	OK	   	- SI hubo problemas.
 */
char 
take_chip (gameADT game);


/*Reparte ("max_number" + 1) fichas a cada jugador.
**
**Retorna:
**	MEM_ERROR	- si se produjo algÃºn error de memoria.
**	EMPTY_POOL	- si no hay suficientes fichas en el pozo.
**	OK		- si no se produjo ningun problema.
 */
char
deal_chips(gameADT game);


/*Función para el uso del comando undo:
**	a) Si el jugador saco una o más fichas después de la última invocación
**	   a la función en el corriente turno, entonces le quita la última que
**	   saco.
**	b) Si no se cumple a) y si en la última invocación a la función,
**	   durante el corriente turno, la misma quito una ficha al jugador,
**	   entonces se la devuelve.
**	c) Si no se tomaron fichas en el corriente turno, no hace nada.
**
**Retorna:
**  	MEM_ERROR	- Si se produjo un error de memoria.
**  	TRUE		- Si termino si problemas.
**	FALSE		- Si termino si problemas, pero no pudo hacer nada
**			  porque en el corriente turno no se sacaron fichas.
 */
char 
undo_command(gameADT game);


/*Estrategia de la computadora: 
**
**Retorna:
**	MEM_ERROR 	- No hay memoria suficiente.
**	EMPTY_POOL	- Si no hay más fichas.
**	END_OF_TURN - jugo la ficha normalmente.
**
**OBS: Al terminar NO setéa el turno siguiente
 */
char 
pc_turn(gameADT game);


/*Juega la ficha más alta entre los jugadores.
**
**Retorna:
**	MEM_ERROR	- si se produjo un error de memoria.
** 	OK		- si no hubo ningún problema.
 */
char
play_highest_chip(gameADT game);


/*Si el jugador tiene el doble principal lo juega.
**
**Retorna:
**	MEM_ERROR	- si se produjo un error de memoria.
**	TRUE	 - Si lo pudo jugar.
**	FALSE	 - Si el jugador no tiene el doble principal
**
**OBS: la función es de orden 1.
 */
char
play_double_main(gameADT game);


/*Guarda los datos necesarios para poder retomar la partida luego.
**
**Retorna:
**	MEM_ERROR - Si no pudo crear el archivo
**	OK	  - Si no hubo ningún problema
**
**Recibe:
**	filename  - Nombre del archivo en el cual se va a guardar la partida.
**		    Si el archivo existe lo sobrescribe.
 */
char
save(gameADT game, char* filename);


/*Carga una partida guardada.
**
**Retorna:
**	NULL	  - Si no pudo cargar la partida
**	game	  - Si no hubo ningún problema
**
**Recibe:
**	filename - Nombre del archivo sobre el cual se van a cargar los datos.
**		   
 */
gameADT
load(char* filename);

#endif
