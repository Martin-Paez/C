#ifndef _DOMINO_BACK_H_
#define _DOMINO_BACK_H_



#define NUMBER_OF_PLAYERS 	 2

/*Posibles retornos de funci�n*/
#define OK			 	 1
#define TRUE			 1
#define FALSE			 0
#define NOTHING			 5
#define EMPTY_POOL		16
#define END_OF_TURN		15
#define ERROR 			-2
#define MEM_ERROR			-3
#define SYNTAX_ERROR		-4

/*Cantidad de n�meros por cada ficha (izquierdo y derecho)*/
#define NUMBERS_PER_CHIP	 2

/*N�mero derecho o izquierdo de una ficha que JAM�S ser� usado.
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

/*Para el uso de la funci�n  game_over()*/
typedef enum {WITHOUT_CHIPS = 12, TRANCA}game_over_t;

/*Para el uso de la funci�n chips()
**OBS: NEXT_CHIP, puede ser cualquier n�mero entero distinto de RESET
 */
typedef enum{RESET, NEXT_CHIP}iterate_t;



/*==================================================================== */

/*Crea una estructura game_t e inicializa los valores
**
**Retorna:
**	NULL - Si se produjo alg�n error.
**
**Recibe:
**	max_number - M�ximo n�mero en una ficha.
**	game_mode  - Mode de juego.
*/
gameADT 
create_game(int max_number, game_mode_t game_mode);


/*Libera la memoria reservada al crear e juego
 */
void 
free_game(gameADT game);


/*Set�a la penalizaci�n al jugador del corriente turno, seg�n el comando
**por el cual se lo va a penalizar.
**
**Recibe:
**   command - Posbles valores:
**
**	'p'(asqui de la letra p)  -  Suma la mitad del valor del número 
**				     máximo en una ficha
**	'd'(asqui de la letra d)  -  Suma el valor del número máximo en 
**				     una ficha.
 */
void 
add_penalty(gameADT game, char command);


/*Retorna el valor de penalizaci�n de un jugador
**
**Retorna en su nombre:
**	Penalizaci�n de un jugador.
**
**Recibe:
**	i - Sub�ndice del jugador sobre el cual se consulta la penalizaci�n.
 */
int 
penalty(gameADT game, player_index_t i);


/*Retorna la cantidad de fichas del tablero � de un jugador en particular seg�n indique
**el par�metro list.
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


/*Informa el estado del pozo (Lleno o vac�o).
**
**Retorna:
**	TRUE  - Si el pozo est� vac�o
**	FALSE - Si todav�a hay fichas en el pozo 0
 */
char 
empty_pool(gameADT game);


/*Indica a quien le toca jugar.
**
**Retorna:
**	 El sub�ndice del jugador en cuesti�n.
 */
player_index_t 
turn(gameADT game);


/*Setea el siguiente turno, inicializa lo pertinente al comando undo
**
**Retorna:
**	El sub�ndice del jugador al cual le toca jugar.
 */
player_index_t
next_turn (gameADT game);


/*Suma el valore de las fichas de un jugador.
**
**Retorna:
**	La suma del valor de todas las fichas en posesi�n del jugador.
**
**Recibe:
**	i - Sub�ndice del jugador sobre el cu�l se consulta.
 */
int 
total_value(gameADT game, player_index_t i);


/*Eval�a si el juego termino.
**
**Retorna:
**	WITHOUT_CHIPS   - si el jugador del corriente no tiene más fichas
** 	TRANCA	  	- si no hay más jugadas posibles
** 	NOTHING         - si no se cumplen las anteriores
 */
char 
check_game_over(gameADT game);


/*Entrega una ficha perteneciente al Tablero o jugador en particular seg�n
**indique el par�metro list.
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
**OBS: La invocaci�n a la funci�n con el par�metro command = RESET entrega 
**en numbers el par ordenado que forma la ficha m�s alta que tenga el jugador
**o que se encuentre en el tablero (dependiendo de list_index_t i). A partir
**de la primer invocaci�n del modo descrito, las siguientes invocaciones se 
**deber�an realizar con el par�metro command = NEXT_CHIP, de forma tal de poder 
**iterar hasta que se reciba el par (END,END) el cual indica que se a recorrido
**toda la mano. Una vez hecho esto para volver a usar la funci�n habr� que  
**invocarla como si fuese por primera vez.
**
**OBS: NEXT_CHIP, puede ser cualquier n�mero entero distinto de RESET
 */
void 
chips(gameADT game, list_index_t list, iterate_t command, 
    char numbers[NUMBERS_PER_CHIP]);


/*Juega una ficha del jugador persona en el tablero.
**
**Retorna en su nombre:
**	SYNTAX_ERROR  - si las fichas no son v�lidas.
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
**	EMPTY_POOL 	- Si no hay m�s fichas en el pozo.
**	OK	   	- SI hubo problemas.
 */
char 
take_chip (gameADT game);


/*Reparte ("max_number" + 1) fichas a cada jugador.
**
**Retorna:
**	MEM_ERROR	- si se produjo algún error de memoria.
**	EMPTY_POOL	- si no hay suficientes fichas en el pozo.
**	OK		- si no se produjo ningun problema.
 */
char
deal_chips(gameADT game);


/*Funci�n para el uso del comando undo:
**	a) Si el jugador saco una o m�s fichas despu�s de la �ltima invocaci�n
**	   a la funci�n en el corriente turno, entonces le quita la �ltima que
**	   saco.
**	b) Si no se cumple a) y si en la �ltima invocaci�n a la funci�n,
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
**	EMPTY_POOL	- Si no hay m�s fichas.
**	END_OF_TURN - jugo la ficha normalmente.
**
**OBS: Al terminar NO set�a el turno siguiente
 */
char 
pc_turn(gameADT game);


/*Juega la ficha m�s alta entre los jugadores.
**
**Retorna:
**	MEM_ERROR	- si se produjo un error de memoria.
** 	OK		- si no hubo ning�n problema.
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
**OBS: la funci�n es de orden 1.
 */
char
play_double_main(gameADT game);


/*Guarda los datos necesarios para poder retomar la partida luego.
**
**Retorna:
**	MEM_ERROR - Si no pudo crear el archivo
**	OK	  - Si no hubo ning�n problema
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
**	game	  - Si no hubo ning�n problema
**
**Recibe:
**	filename - Nombre del archivo sobre el cual se van a cargar los datos.
**		   
 */
gameADT
load(char* filename);

#endif
