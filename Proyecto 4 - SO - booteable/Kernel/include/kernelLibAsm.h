#ifndef _KERNEL_LIB_ASM_
#define _KERNEL_LIB_ASM_

/* Idem kasm.h */
#include "./defs.h"


unsigned int    _read_msw();

void            _lidt (IDTR *idtr);

/* Escribe mascara de PIC1 */
void		_mascaraPIC1 (byte mascara);  

/* Escribe mascara de PIC2 */
void		_mascaraPIC2 (byte mascara);  

/* Deshabilita interrupciones  */
void		_Cli(void);   

/* Habilita interrupciones  */
void		_Sti(void);	 

/* Timer tick */
void		_int_08_handler();      

/* Teclado */
void		_int_80_handler();      

/* Para el desarrollador */
void		_debug (void);

#endif