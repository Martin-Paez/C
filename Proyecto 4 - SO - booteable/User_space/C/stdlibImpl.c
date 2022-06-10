#include "./include/stdlibImpl.h"
#include "./include/errnoImpl.h"
#include "./include/errno-baseImpl.h"
#include "./include/stdioImpl.h"
#include "./include/libcImpl.h"

/* Convierte el char que representa un numero entre 0 y 9
 * a entero.
 */
int toDigit(char symbol) {
        static char *symbols[] = {"0", "1", "2", "3", "4", "5", "6", "7"
			      , "8","9"};
        int i = 0;
        while ( symbol != *symbols[i])
                if ( ++i == 10 )
                        return -1;
        return i;
}

/* DEFINIDA EN CTYPE.H 
int isdigit(int c) {
        return toBaseXDigit(c, 10) + 1; 
}*/
/*
 * Si el string a convertir tiene caracteres invalidos se setea errno en 
 * EINVAL (Ver http://linux.die.net/man/3/errno ).
 */
long int strtolImpl(char *nptr, char **endptr) {
	if ( *nptr == 0 ) {
		errnoImpl = EINVALImpl;
		return 0;
	}
        char *itr = nptr;
        long ret = 0;
        int sign = 1;

        if ( *itr == '-' ) {
                sign = -1;
                itr++;
        } else if ( *itr == '+' )
                itr++;

                int i = 0, loop = 0, plus;
                do {
                       plus = toDigit(*itr);
                        if ( plus < 0 ) {
				if ( loop == 0 )
					errnoImpl = EINVALImpl;
				return 0;
                       } else {
				itr++;
                                ret = ret*10 + plus;
				loop++;
			}
                } while ( loop && *itr != 0x0 && !isspaceImpl(*itr) );
                *endptr = itr;

        return ret * sign;
}

/* Obtenido de la siguiente direccion:
 *      http://stackoverflow.com/questions/9655202
 * 		/how-to-convert-integer-to-string-$
 */
char* itoaImpl(int i, char b[]){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    }while(i); 
    return b;
}