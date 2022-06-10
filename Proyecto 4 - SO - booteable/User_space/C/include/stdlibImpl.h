#ifndef _STDLIB_
#define _STDLIB_

#include "./ctypeImpl.h"

long int strtolImpl(char *nptr, char **endptr);

char* itoaImpl(int i, char b[]);

/* Convierte el char que representa un numero entre 0 y 9
 * a entero.
 */
int toDigit(char symbol);

#endif