#ifndef _STDIO_
#define _STDIO_

#include "./stdlibImpl.h"
#include "./ctypeImpl.h"
#include "./stringImpl.h"
#include "./stdargImpl.h"
#include "./libioImpl.h"


#define EOF     (-1)
#define MAX_COMMAND_LENGTH 100

/* Obtenido de la biblioteca standard de C */

#include "./libioImpl.h"

typedef struct _IO_FILEImpl FILEImpl;		/* Solo para ajustarme a la API. */

/* Standard streams.  */
extern struct _IO_FILEImpl *stdinImpl;          /* Standard input stream.  */
extern struct _IO_FILEImpl *stdoutImpl;         /* Standard output stream.  */
extern struct _IO_FILEImpl *optoutImpl;         /* Optional output stream.  */
extern struct _IO_FILEImpl *stderrImpl;         /* Standard error output stream. */

/* C89/C99 say they're macros.  Make them happy.  */
#define stdinImpl stdinImpl
#define stdoutImpl stdoutImpl
#define stderrImpl stderrImpl

/* Obtenido de la API de C. En la API aparece dentro de un #ifdef. Para 
 * el tpe basta con colocar esto y definir la funcion en stdio.c.
 * Me parecio conveniente colocar la implementacion de la funcion en 
 * libio.h , ya que es alli donde se define _IO_FILE y donde se encuentran
 * las funciones read() y write().
 */
extern int filenoImpl (FILEImpl *__stream);

/* Coloca un caracter al final del stream recivido como parametro
 * Si se produjo un error retorna EOF
 */
int putcImpl(int c, FILEImpl *stream);
/* Retorna un unsigned char leido del stream. PUede ser utilizado
 * como un iterador.
 * Si se produjo un error o se llego al final de stream retorna EOF.
 */
int getcImpl(FILEImpl *stream);

int putsImpl( const char * str );

int scanfImpl(const char *fmt, ...);

int printfImpl(const char *fmt, ...);

/* Lee del buffer del teclado un solo elemento (sin necesidad de presinar
 * enter)
 */
char getch(void);

/* Imprime un numero entero */
void puti(int i);

#endif
