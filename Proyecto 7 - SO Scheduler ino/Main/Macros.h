#ifndef MACROS_H
#define MACROS_H

/***************************************************************************/
/* ------------------------------ Macros.h --------------------------------*/
/* --------------------------- MACROS UTILES ------------------------------*/
/* ------------------------------------------------------------------------*/
/***************************************************************************/

#define PIN(n)  n

#define NOT     !
#define BOOL    int
#define TRUE    1
#define FALSE   0
#define BYTE    char
#define uBYTE   unsigned char

void *voidPtr;
#define SIZE_PTR(vec)   ((int) ( sizeof(vec) / sizeof(voidPtr))  )
#define SIZE(vec)       ((int) ( sizeof(vec) / sizeof(vec[0]) ) )

#endif
