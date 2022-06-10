#include "./include/stringImpl.h"

size_tImpl strlenImpl(const char* cs) {
	int i = -1;
	while ( cs[++i] != 0x0 );
	return i;
}

int  strcmpImpl(char * a, char * b) {

        char *i = a, *j = b;

        while ( *i != 0x0 && *j != 0x0 && *i == *j ) {
                i++;
                j++;
        }

        if ( *i == *j )
                return 0;
        return *i - *j;
}
