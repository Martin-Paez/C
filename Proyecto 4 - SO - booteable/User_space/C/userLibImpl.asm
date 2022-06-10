GLOBAL  syscallImpl

SECTION .text


EXTERN  int80
EXTERN  int08

syscallImpl:
  
  push	ebp
  mov	ebp, esp
  
  mov	eax, [ebp+8]
  mov	ebx, [ebp+12]
  mov	ecx, [ebp+16]
  mov	edx, [ebp+20]
  int	80h
  
  pop 	ebp
  ret