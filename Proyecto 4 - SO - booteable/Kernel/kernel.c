#include "./include/kernelLibAsm.h"
#include "./include/kernelLibC.h"
#include "./include/defs.h"
#include "./include/keyboardApi.h"
#include "../Drivers/Keyboard_Driver/include/kbDriverAsm.h"
#include "../User_space/shell.h"
#include "../User_space/C/include/stdioImpl.h"
#include "./include/videoApi.h"


DESCR_INT idt[0x90];			/* IDT de 90 entradas*/
IDTR idtr;				/* IDTR */


/**********************************************
kmain() 
Punto de entrada de cóo C.
*************************************************/

kmain() 
{
	int i,num;


// CARGA DE IDT CON LA RUTINA DE ATENCION DE IRQ0 

        setup_IDT_entry (&idt[0x08], 0x08
		 , (dword)&_int_08_handler, ACS_INT, 0);
        setup_IDT_entry (&idt[0x09], 0x08
		 , (dword)&_keyboardHandler, ACS_INT, 0);
	setup_IDT_entry (&idt[0x80], 0x08
		 , (dword)&_int_80_handler, ACS_INT, 0);
	
// Carga de IDTR    

	idtr.base = 0;
	idtr.base +=(dword) &idt;
	idtr.limit = sizeof(idt)-1;
	
	_lidt (&idtr);	

	_Cli();
// Habilito interrupcion de timer tick

        _mascaraPIC1(0xFC);
        _mascaraPIC2(0xFF);
        
	_Sti();

	loadDrivers();

	stdinImpl->fd = STDIN_FILENO;
	stdoutImpl->fd = STDOUT_FILENO;
	optoutImpl->fd = OPTOUT_FILENO;
	
	sys_clear_screen();

	shell();

}



