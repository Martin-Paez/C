#include "./include/kbDriverC.h"
#include "./include/kbDriverAsm.h"
#include "../../User_space/C/include/stdioImpl.h"
#include "../../User_space/C/include/stdlibImpl.h"

static  struct kbBuffer_t {
  char	start[KB_BUFFER_SIZE];
  char	*next;
  char	*end;
  int	size;
  char	*last;
} kbBuf;


static int init = 1;

void kbInitialize(void) {
  kbBuf.size = 0;
  kbBuf.next = kbBuf.start;
  kbBuf.end = kbBuf.start + KB_BUFFER_SIZE;
  kbBuf.last = kbBuf.next;
  init = 0;
// INICIALIZAR LOS LEDS
// INICIALIZAR CURSOR
}


#define	UNDEFINED	0
#define	CAPSLOCK	1
#define	NUMLOCK		2	
#define SHIFT		3
#define CTRL		4

#define DISABLED	0
#define ENABLED		1

#define LOWER_CASE	0
#define UPPER_CASE	1  

#define PRESSED		1
#define RELEASED	-1

#define OFFSET		0X80

#define DO_NOTHING	UNDEFINED  

static char shiftModifiedKeys[89] = {
			    0, 0, ('!'-49), ('@'-50), ('#'-51), ('$'-52), ('%'-53),
			    ('^'-54), ('&'-55), ('*'-56), ('('-57), (')'-48),
			    ('_'-'-'), ('+'-'='), 0, 0, 0, 0, 0, 0, 0,
			    0, 0, 0, 0, 0, ('{'-'['), ('}'-']'), 0, 0 , 0, 0,
			    0, 0, 0, 0, 0, 0, 0, (':'-';'), ('"'-'\''), ('~'-'`'),
			    0, ('|'-'\\'), 0, 0, 0, 0, 0, 0, 0, ('<'-','),('>'-'.'),
			    ('?'-'/'), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
			  };
static char table[2][89] = { 
			  {
			    UNDEFINED, 27, 49, 50, 51, 52, 53, 54, 55, 56, 57,
			    48, '-', '=', '\b', '\t', 'q','w','e','r','t' ,
			    'y','u','i','o','p','[',']','\n', CTRL,'a','s',
			    'd','f','g','h','j','k','l',';','\'','`',  SHIFT,
			    '\\', 'z','x','c','v','b','n','m',',','.', '/',
			    SHIFT, '*',  UNDEFINED, ' ', CAPSLOCK, UNDEFINED,
			    UNDEFINED, UNDEFINED, UNDEFINED, UNDEFINED,UNDEFINED
			    ,UNDEFINED, UNDEFINED, UNDEFINED, UNDEFINED,
			    NUMLOCK,UNDEFINED,'7','8', '9', '-', '4',
			    '5', '6', '+', '1', '2', '3', '0', '.'
			  },
			  {
			    UNDEFINED, 27, 49, 50, 51, 52, 53, 54, 55, 56, 57,
			    48, '-', '=', '\b', '\t', 'Q','W','E','R','T' ,
			    'Y','U','I','O','P','[',']','\n', CTRL,'A','S',
			    'D','F','G','H','J','K','L',';','\'','`',  SHIFT,
			    '\\', 'Z','X','C','V','B','N','M',',','.', '/',
			    SHIFT, '*',  UNDEFINED, ' ', CAPSLOCK, UNDEFINED,
			    UNDEFINED, UNDEFINED, UNDEFINED, UNDEFINED,UNDEFINED
			    ,UNDEFINED, UNDEFINED, UNDEFINED, UNDEFINED,
			    NUMLOCK,UNDEFINED,'7', '8', '9', '-',
			    '4', '5', '6', '+', '1', '2', '3', '0', '.'
			  }
			};

    /* Actuan sobre la toda lista de todos los scancodes */	
      #define isNowPressed	( code > 0 )/*(code > 0x00 && code < 0x46)*/
      #define isNowReleased	( code < 0 )/*(code > 0x80 && code < 0xD4)*/

    /* Actua SOLO sobre la lista de scancodes que indican las teclas 
     * presionadas */
      #define isNumpadKey	(code > 0x46 && code != 0x4E && code != 0x53 \
				  && code != 0x4A ) 

      /* Actuan bajo la comprension de las variables locales */
      #define turn(K)		((specialKey[K-1]==ENABLED)?(specialKey[K-1]=DISABLED):(specialKey[K-1]=ENABLED))	
      #define isTurnOn(K)	( specialKey[K-1] == ENABLED )
      #define turnCase		((letters==LOWER_CASE)?(letters=UPPER_CASE):(letters=LOWER_CASE))
      
      #define previousState(CODE)	  ((table[letters][CODE]<0)?RELEASED:PRESSED)  
      #define isMapedCode(CODE)		  (CODE>0 && CODE<89)
      #define getKey(CODE)		  (table[letters][CODE] * previousState(CODE))	// Le quito el signo (estado)
      #define updateKeyState(CODE)	  ( table[letters][CODE] *= (-1) )
      #define showHideKeyValue(KEY, CODE) (KEY = KEY + shiftModifiedKeys[CODE])

/* Observacion (sobre la implementacion):
 * Se decide utilizar returns a lo largo de toda la funcion ya que lo que hace
 * es basicamente buscar paso por paso en que opcion se encuentra y realizar
 * la "tarea" correspondiente. Es similar a como funciona un switch pero con 
 * reglas mas complejas para cada caso.
 */
static char scancodeToChar(char code) {

  /* Aclaracion:
   * Se opto por usar dos metodos para detectar que teclas estan presionadas
   * o activas (como capslock o numlock). Por un lado para algunos casos se
   * usa un signo positivo o negativo el la matris 'table' y por otro lado para
   * algunas teclas se usa el array 'specialKeys'. 
   */  
  static int specialKey[4] = {DISABLED, DISABLED, DISABLED, DISABLED};	/* INICIALIZAR LOS LEDS!!!!!!!!!! */
  static int letters = LOWER_CASE;
  static int state;

/* Reduzco la "tabla" original que tiene mas teclas y que usa distintas 
 * entradas para teclas presionadas y no presionadas.
 */

int key;
  if ( isNowReleased ) {
	state = RELEASED;
	code += OFFSET;
  } else if ( ! isNowPressed ) {
    	return UNDEFINED;	// CASO : Tecla no mapeada
  } else
	state = PRESSED;

  if ( ! isMapedCode(code) )
	return UNDEFINED;

  key = getKey(code);
  updateKeyState(code);		// CASO : Se cambio el estado de la tecla

  if ( key == CTRL ) {
	turn(CTRL);
	return DO_NOTHING;
  }
  
  if ( isNumpadKey && !isTurnOn(NUMLOCK) ){
     return DO_NOTHING /* == UNDEFINED */;	// CASO : NumLock apagado
   }

  int isNumLock = key==NUMLOCK;
  if ( key == SHIFT || 
	(state == PRESSED && (key == CAPSLOCK || isNumLock)) ) {
	if (!isNumLock)		
		turnCase;
	turn(key);
	/* turnLeds(key); */
	return DO_NOTHING; 
  } else if ( state == RELEASED ) 
	return DO_NOTHING; /* Ojo con '/b' al cambiar la implementacion */
	
  if ( isTurnOn(SHIFT) )
	showHideKeyValue(key, code);

  /* Segun lo dicho por la catedra se implementa esto desde aca */
#define sysShortcut	2
  if ( isTurnOn(CTRL) && (key == 'R' || key == 'r') ){	
	return sysShortcut;
  }
  return key;
}

/* Segun lo dicho por la catedra se implementa esto desde aca */ 
int printRegistersRap(int argCount, ...) {

	#define BUF_SIZE	(80 * 10)
	#define MID_LINE	(40)
	char buf[BUF_SIZE];
	static char *regs[] = { "eip=", "gs=", "fs=", "es=", "ds=", "ss=",
			 "cs=", "edi=", "esi=", "ebp=", "esp=", "ebx=",
			 "edx=", "ecx=", "eax=" };
	
        va_list ap;
        va_start (ap, argCount);

	clearScreen(filenoImpl(optoutImpl));
	int i=0, regCount=0, j=0;
	while ( i < BUF_SIZE && regCount < argCount) {
		while ( regs[regCount][j] != 0x0 )
			buf[i++] = regs[regCount][j++];
		j = 0;
		itoaImpl(va_arg( ap, int),buf+i);
		regCount++;
		while ( buf[i] != 0x0 )
			i++;
		for ( ; i < MID_LINE * regCount; i++ )
			buf[i] = ' ';
	}
	while ( i < BUF_SIZE)
		buf[i++] = ' ';
	va_end(ap);

	if ( writeImpl(filenoImpl(optoutImpl), buf, BUF_SIZE)<0 ) \
		      return EOF;
	return 1;
}

/* Segun lo dicho por la catedra se implementa esto desde aca */ 
int printRegisters(int eip, int gs, int fs, int es, int ds, int ss,
	int cs, int edi, int esi, int ebp, int esp, int ebx, int edx,
	int ecx, int eax) {

	return printRegistersRap(15, eip, gs, fs, es, ds, ss, cs, edi, esi,
		ebp, esp, ebx, edx, ecx, eax);
}

int keyPressed(char scancode) {
  //printfImpl("\n \n\nDirect received scancode = %d", scancode);
  int key = scancodeToChar(scancode);
  if ( key == UNDEFINED || key == sysShortcut )
	return key;

 // printfImpl(" \n Tecla presinada. Size = %d, scancode = %d \n", kbBuf.size, scancode);
  /*if ( key == '\b' ) {
	if ( kbBuf.size > 0 ) {
		if ( kbBuf.next-- == kbBuf.start )
			kbBuf.next = kbBuf.end;
		kbBuf.size--;
  	}
  } else {  */
        *(kbBuf.next) = key;
	if ( ++kbBuf.next == kbBuf.end )
	   kbBuf.next = kbBuf.start;
	if (kbBuf.next == kbBuf.last && ++kbBuf.last == kbBuf.end )
	    kbBuf.last = kbBuf.start;
	if (kbBuf.size < KB_BUFFER_SIZE)
	  kbBuf.size++;
	return DO_NOTHING;
  //}
 // printfImpl("Primer valor en el buffer = %c \n", *kbBuf.last);
}

/* Siempre da OK. No se chequea nada (se considera la implementacion por fuera
 * de las espectativas del tpe). 
 * El prototipo de la funcion es similar al usado en el kernel de Linux. En el
 * kernel de Linux se usa "const void __user *addr", donde __user es una macro
 * que simplemente sirve para indicar al programador que es una direccion enviada
 * por el usurio (y que por lo tanto hay que tomar los recaudos necesarios).
 */
static inline int access_ok(int type, void * addr, unsigned long size) {
  return 1;
}

/* La idea es que como se esta tratando de acceder a posiciones de memoria que
 * fueron dadas por el usuario, hay que tener los recaudos necesarios para 
 * evitar que sean posiciones de memoria invalidas. Sobre todo si es el kernel
 * el que hace el llamado a esta funcion (como se espera), ya que desde el kernel
 * se puede acceder a toda la memoria.
 */
int kbRead(char *buff, int size) {
  
  if ( access_ok(sizeof(char), buff, size) ) {
//printfImpl("SIZE : %d",kbBuf.size);
    if ( size > kbBuf.size )
     size = kbBuf.size;
    int i = size;
    while ( i-- > 0 ) {
      if( kbBuf.last == kbBuf.end )
	kbBuf.last = kbBuf.start;
	*(buff++) = *(kbBuf.last++);
    }
    /* Como se utilizo access_ok() y kbBuff.last es "conocida" asumo que no 
     * no se produjeron errores y por lo tanto modifico bkBufer.size al final.
     */
    kbBuf.size -= size;
    
	    return size;
    
  }
  return 0;
}

void keyboardHandler(void) {
	_keyboardHandler();
	__asm__("iret");
}
