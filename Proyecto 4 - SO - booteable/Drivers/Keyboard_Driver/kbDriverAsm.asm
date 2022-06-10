GLOBAL  _keyboardHandler

EXTERN	printRegisters
EXTERN  keyPressed

SECTION .text

_keyboardHandler:
	push	ds	; pushea 4 bytes
	push	es	; pushea 4 bytes
	pusha		; pushea 32 

	push	eax
	push	ecx
	push	edx
	push	ebx
	push	esp
	push	ebp
	push	esi
	push	edi
	push	cs
	push	ss
	push	ds
	push	es
	push	fs
	push	gs
	mov	eax,[esp+96]	; pusheo eip
	push	eax

	mov	eax,0h
	in	al, 60h
	push	eax
        call    keyPressed
	pop	ecx		; Pop no modifica los flags
	cmp	eax, SHORTCUT	; Valor de retorno en eax
	jne	finish	
	call	printRegisters  ; En el stack estan los parametros (registros)
finish:
	mov	al,20h		; Envio de EOI generico al PIC
	out	20h,al
	
	add	esp,60		; 15*4bytes = 60

	popa
	pop 	es
	pop 	ds
	iret 

_showStack :
  mov	ax,20h
  add	ecx, 8h
loop :
  mov	edx,[ecx]
  add	ecx,4h
  add	edx, 30h
  mov	[ebx],dl
  add	ebx,2h
  dec	ax
  cmp	ax,0h
  jne	loop
  ret

section .data
	SHORTCUT	equ	2h

