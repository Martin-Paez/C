#ifndef _LIBC_
#define _LIBC_

/* Para poder usar arrays al estilo : typo nombre[cantidad]  
 * o  typo nombre[] = "bla bla".
 * Esto es necesario si los arrays estan dentro de funciones
 */
void __stack_chk_fail();

#endif