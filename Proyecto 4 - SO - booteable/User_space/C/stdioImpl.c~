#include "./include/stdioImpl.h"
#include "./include/errnoImpl.h"
#include "./include/errno-baseImpl.h"

int errnoImpl = 0;

struct _IO_FILEImpl __stdinImpl, __stdoutImpl, __optoutImpl, __stderrImpl ;
struct _IO_FILEImpl *stdinImpl = &__stdinImpl;
struct _IO_FILEImpl *stdoutImpl = &__stdoutImpl;
struct _IO_FILEImpl *optoutImpl = &__optoutImpl;
struct _IO_FILEImpl *stderrImpl = &__stderrImpl;


/* En la implementacion real se supone que errno arranca en cero.
int * __errno;*/

/* Coloca un caracter al final del stream recivido como parametro
 * Si se produjo un error retorna EOF. 
 */
int putcImpl(int c, FILEImpl *stream) {
  if ( writeImpl(filenoImpl(stream) , &c, 1) < 0 )
    return -1;
  return 1;
}


/* Retorna un unsigned char leido del stream. Puede ser utilizado
 * como un iterador.
 * Si se produjo un error o se llego al final de stream retorna EOF.
 */
int getcImpl(FILEImpl *stream) {
  char c = ' ', o = ' ';
  int i = 0, j;
  do {
    if ( (j = readImpl( filenoImpl(stream), &c, 1)) < 0 )
      return -1;
    else if ( i == 0 && j != 0 ) {
      i = 1; //Ojo no se puede usar i como contador por el overflow
      o = c;
      putcImpl(o,stdoutImpl);
    }
    j = 0;
  } while ( c != '\n' );
  return o;
}

/* Imprime un numero entero */
void puti(int i) {
  printfImpl("%d", i);
}

/* Lee del buffer del teclado un solo elemento (sin necesidad de presinar
 * enter)
 */
char readch(void) {
  char c;
  while ( readImpl(filenoImpl(stdinImpl), &c, 1) <= 0 );
  return c;
}

/*
 * IMPORTANTE:
 * 	Para leer strings , se leen como maximo tantos valores como posiciones 
 * 	de memoria hallan desde el puntero a char que se recive como uno de 
 * 	los argumentos variables hasta el primer 0x0.
 */
/* Para escribir esta funcion se opto primero por buscar un codigo en
 * internet que hiciera lo necesairio. Si bien se encontro uno, de a poco
 * se lo fue modificando para arreglar errores, para agregarle funcionalidad
 * y para adaptarlo a la falta de funciones de la API de C. Como producto
 * final se obtuvo la siguiente funcion que es practicamente una funcion 
 * distinta pero con la misma "estructura".
 * 
 * Observacion: el codigo original es de la funcion sscanf (no scanf). Por
 * lo tanto ademas de lo mensionado anteriormente se lo adapto para que realice
 * lo que debe realizar la funcion scanf.
 * 
 * El codigo original se puede encontrar en la siguiente direccion:
 * https://code.google.com/p/minilib-c/source/browse/trunk/stdio/scanf.c?r=8 
 */
/*Observacion
 * 	El formato aceptado solo reconoce %d y %s. Se tomo esta decision ya
 * que la logica de implementar un formato mas complejo no aportaba a los 
 * conocimientos que se estan avaluendo en la materia. Es una implementacion
 * bastante "acomodada" a la implementacion real y que brinda suficientes
 * herramientas como para trabajar. 
 */
/* Observacion: 
 *	La instruccion sscanf("10   14ia","%d %d %d",&a, &c) real
 *	compila con warning (por faltar un parametro) y guarda 10
 *	 en 'a' y 14 en 'c'.
 */
int scanfImpl(const char *fmt, ...){

/* TODO - Modularizar ( Leer comando - Cargar valores ) */
  
        char str[MAX_COMMAND_LENGTH];
	char *str_ptr = str;
        char c, i=0;

        while ( (c = readch()) != '\n' ) {
		if ( c == '\b' ) {
			if ( i > 0 ) {
				i--;
				putcImpl(c, stdoutImpl);
			}
		} else if ( i < MAX_COMMAND_LENGTH ) {
                        str_ptr[i++] = c;
			putcImpl(c, stdoutImpl);
		}
         }
        
	putcImpl(c, stdoutImpl);
        str_ptr[i] = 0x0;
        int ret;
        va_list ap;
        char *format_ptr = (char*)fmt;

        int *p_int;
        long *p_long;
	char *p_str;

        ret = 0;

        va_start (ap, fmt);

	char loop = 1;
        while (*format_ptr != 0x0 && *str_ptr != 0x0 && loop )
        {

	  if (*format_ptr == '%')
                {
                        format_ptr++;

                        if (*format_ptr != 0x0)
                        {
                                switch (*format_ptr)
                                {
                                case 'd':
                                case 'i':
                                        p_int = va_arg( ap, int *);
					errnoImpl = 0;
                                        int aux = (int)strtolImpl (str_ptr, &str_ptr);
                                        if ( errnoImpl == EINVALImpl )
						loop = 0;
					else {
						*p_int = aux;
						if ( isspaceImpl(str_ptr[0]) )
							str_ptr--;
						ret ++;
					}
                                        break;
                               case 's':
					p_str = va_arg( ap, char *);
					int i=0, s=strlenImpl(p_str)-1;
					while ( i<s && *str_ptr != 0x0 && !isspaceImpl(*str_ptr) )
					  p_str[i++] = *(str_ptr++);
					p_str[i] = 0x0;
					break;
			       case 'c':
					p_str = (char*) va_arg(ap, int*);
					*p_str = *(str_ptr++);
					break;
				}
                        }
                }

		while ( *(++str_ptr) != 0x0 && isspaceImpl(*str_ptr) );
		while ( *(++format_ptr) != 0x0 && isspaceImpl(*format_ptr) );
        }

        va_end (ap);

        if (loop == 0)
		return EOF;
        return ret;
#undef MAX_COMMAND_LENGTH
}

int printfImpl(const char *fmt, ...)
{
        int ret = 0;
        va_list ap;
        va_start (ap, fmt);
	char *end = (char*)fmt;
	/* Se decide hacer str constante ya que no se puede alocar memoria */
        char str[1000];
	int j = -1, p_int, s;
	char *p_str, c_ptr;

#define WR(BUFF, SIZE) if ( writeImpl(filenoImpl(stdoutImpl), BUFF, SIZE)<0 ) \
		      return EOF;
	  
        while (*end != 0x0) {
                if (*end == '%') {
			WR(str,++j);
			ret += j;
			j = -1;
			switch (*(++end)) {
			      case 'd':
			      case 'i':
			      	      p_int = va_arg( ap, int);
				      itoaImpl(p_int, str);
				      s = strlenImpl(str);
				      ret += s;
				      WR(str, s);
				      break;
			      case 's':
				      p_str = va_arg( ap, char *);
				      s = strlenImpl(p_str);
				      ret += s;
				      WR(p_str, s);
				      break;
			      case 'c':
				      putcImpl((char)va_arg( ap, int),stdoutImpl);
				      ret++;
				      break;
				      
			      default :
				      str[++j] = *(--end);
			}
			end++;
                } else 
			str[++j] = *end++;
	}
	WR(str,++j);
#undef WR
        va_end (ap);

        return ret;
}
