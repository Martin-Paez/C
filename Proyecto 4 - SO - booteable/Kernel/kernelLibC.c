#include "./include/kernelLibC.h"
#include "../User_space/C/include/libioImpl.h"
#include "../User_space/C/include/stdioImpl.h"
#include "./include/defs.h"

/* ESTO DEBERIA TENER OTRA FORMA DE HACERSE EN VEZ DE 
 * PONER ESTE INCLUDE, PORQUE SI QUIERO CAMBIAR LOS 
 * DRIVERS COMO HAGO??
 */
#include "./include/keyboardApi.h"
#include "./include/videoApi.h"

/* Los prototipos se obtuvieron de codigo real pero las 
 * implementaciones no. 
 */ 

#define N_SYSCALLS 30
static int  (*syscallHandlers[N_SYSCALLS])(unsigned int fd
    ,  void  *buffer,  size_tImpl  nbyte);

int  int80(int syscall, int  handle,  void  *buffer,  int  nbyte) {
  if ( syscall < N_SYSCALLS && (long)syscallHandlers[syscall] != NULL )
      return (*syscallHandlers[syscall])(handle, buffer, nbyte);
}

long sys_read(unsigned int fd, char __user *buf,
		     size_tImpl count){  
  switch(fd) {
    case STDIN_FILENO:
	return kbRead(buf, count);
    case STDOUT_FILENO:
      return -1;
  }
  return -1;
}

long sys_write(unsigned int fd, char __user *buf
		      , size_tImpl count) {
//vWrite(buf, count);

  switch(fd) {
    case STDOUT_FILENO:
    case OPTOUT_FILENO:	
      return vWrite(fd, buf, count);
    case STDIN_FILENO:
      return -1;
  }  
  return -1;
}

static void (*timertickListeners[MAX_SIMULTANEOUS_TIMERTICK_LISTENERS])();
static int timerListeners = 0;

void int08() {
  int i=0;
  for (i=0; i<timerListeners; i++) {
    (*timertickListeners[i])();
  }
}

int addTimertickListener(void (*func)() ) {
  if ( timerListeners == MAX_SIMULTANEOUS_TIMERTICK_LISTENERS )
    return -1;

  timertickListeners[timerListeners++] = func;
  return timerListeners - 1;
}

#define SYS_TYPE ( int(*)(unsigned int,void*,size_tImpl) )

void loadDrivers() {
  int i;
  for ( i = 0; i< N_SYSCALLS; i++ )
    syscallHandlers[i] = NULL;
  syscallHandlers[__NR_READ] = SYS_TYPE sys_read;
  syscallHandlers[__NR_WRITE] = SYS_TYPE sys_write;
  kbInitialize();
  videoInitialize(0xA);
}  


/***************************************************************
*k_clear_screen
*
* Borra la pantalla en modo texto color.
****************************************************************/

void sys_clear_screen(int fd) 
{
	clearScreen(fd);
}

/* Codigo del pretpe */

/***************************************************************
*setup_IDT_entry
* Inicializa un descriptor de la IDT
*
*Recibe: Puntero a elemento de la IDT
*	 Selector a cargar en el descriptor de interrupcion
*	 Puntero a rutina de atencion de interrupcion	
*	 Derechos de acceso del segmento
*	 Cero
****************************************************************/



void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset
		      , byte access, byte cero) {
  item->selector = selector;
  item->offset_l = offset & 0xFFFF;
  item->offset_h = offset >> 16;
  item->access = access;
  item->cero = cero;
}

