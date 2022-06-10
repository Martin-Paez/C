#include "./include/libioImpl.h"
#include "../../Kernel/include/unistd.h"
#include "./include/errnoImpl.h"
#include "./include/errno-baseImpl.h"


/* Intenta escribir siguientes 'nbyte' bytes del archivo asociado 
 * al 'handle' en el 'buffer'.
 * Retorna la cantidad de caracteres leidos (si se da end_of_file
 * retorna cero) o -1 si se produjo un error.
 */
int  readImpl(  int  fd,  void  *buffer,  int  nbyte ) {
//#include "./include/stdioImpl.h"
  return syscallImpl(__SYS_READImpl, fd, buffer, nbyte);
}

/* Intenta escribir siguientes 'nbyte' bytes del 'buffer' en el
 * archivo asociado al 'handle'. 
 * Retorna la cantidad de caracteres escritos o -1 si se produjo
 * un error.
 */
int  writeImpl(  int  fd,  void  *buffer,  int  nbyte  ) {
  return syscallImpl(__SYS_WRITEImpl, fd, buffer, nbyte);
}

int filenoImpl (FILEImpl *stream) {
  
/*  if ( stream == NULL ) {
      errnoImpl = EINVALImpl;
      return -1;
  }
*/
  return stream->fd;
  
}