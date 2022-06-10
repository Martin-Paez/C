#ifndef _SYSCALL_
#define _SYSCALL_

#include "../../../Kernel/include/unistd.h"

#define __SYS_READImpl	__NR_READ
#define __SYS_WRITEImpl	__NR_WRITE
#define __SYS_SECONDS	__NR_SECONDS
#define __SYS_HOURS	__NR_HOURS

/* Arreglar el codigo para que reciva (int , void *, void *, void *) */
int syscallImpl(int sysCall, int p1, char * p2, int p3);

#endif