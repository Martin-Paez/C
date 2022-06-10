#ifndef _KB_DRIVER_C_
#define _KB_DRIVER_C_
  
#define KB_BUFFER_SIZE 1

int keyPressed(char scancode);

int printRegisters(int gs, int fs, int es, int ds, int ss,int cs,
	int edi, int esi, int ebp, int esp, int ebx, int edx,
	int ecx, int eax, int eip);

#endif
