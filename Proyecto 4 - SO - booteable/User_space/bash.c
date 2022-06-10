#include "../Kernel/include/kernelLibC.h"
#include "../Kernel/include/videoApi.h"
#include "./C/include/stringImpl.h"
#include "./C/include/stdioImpl.h"
#include "../Kernel/include/kernelLibC.h"

void cls(void) {
	sys_clear_screen(filenoImpl(stdoutImpl));
}

void bash(char *command) {
  /* Observacion : podria aprobecharse que strcmp retorna tambien
   * si un string es mayor que el otro para acceder mas rapidamente
   * a la opcion buscada. Sin embargo para el tpe no tiene sentido
   * complicarse con cosas por el estilo
   */
  if ( strcmpImpl(command, "cls") == 0 )
  	cls();
}

