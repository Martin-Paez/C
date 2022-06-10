#ifndef _LIBIO_
#define _LIBIO_

#include "./stdioImpl.h"
#include "./syscallImpl.h"

#define NULL	0L

struct _IO_FILEImpl {	 /* Solo para ajustarme a la API */
  char	*start;
  char	*next;
  char 	*end;
  int	size;
  int	fd;
};

/* Intenta escribir siguientes 'nbyte' bytes del archivo asociado 
 * al 'handle' en el 'buffer'.
 * Retorna la cantidad de caracteres leidos (si se da end_of_file
 * retorna cero) o -1 si se produjo un error.
 */
int  readImpl(  int  handle,  void  *buffer,  int  nbyte );


/* Intenta escribir siguientes 'nbyte' bytes del 'buffer' en el
 * archivo asociado al 'handle'. 
 * Retorna la cantidad de caracteres escritos o -1 si se produjo
 * un error.
 */
int  writeImpl(  int  handle,  void  *buffer,  int  nbyte  );

#endif