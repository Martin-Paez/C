GLOBAL  _read_msw
GLOBAL	_lidt
GLOBAL  _int_08_handler, _int_80_handler, 
GLOBAL  _mascaraPIC1,_mascaraPIC2,_Cli,_Sti
GLOBAL  _debug

EXTERN  int80
EXTERN  int08

SECTION .text


_int_08_handler:
	
	push	ebp
	mov	ebp, esp
	push	ds
	push	es
	pusha
	
	call	int08
	mov	al,20h			; Envio de EOI generico al PIC
	out	20h,al
	
	popa
	pop 	es
	pop 	ds
	pop	ebp
	iret

_int_80_handler:
	
	push	ebp
	mov	ebp, esp
	push	ds
	push	es
	
	push	edx
	push	ecx
	push	ebx
	push	eax   
	call    int80
	add	esp,16
	push 	eax
	mov	al,20h			; Envio de EOI generico al PIC
	out	20h,al
	pop 	eax
	
	pop 	es
	pop 	ds
	pop	ebp
	iret
  
_Cli:
	cli			; limpia flag de interrupciones
	ret

_Sti:
	sti			; habilita interrupciones por flag
	ret

; Escribe mascara del PIC 1_lidt (&idtr);	
_mascaraPIC1:			
	push    ebp
        mov     ebp, esp
        mov     ax, [ss:ebp+8]  ; ax = mascara de 16 bits
        out	21h,al
        pop     ebp
        retn

; Escribe mascara del PIC 2_lidt (&idtr);	
_mascaraPIC2:			
	push    ebp
        mov     ebp, esp
        mov     ax, [ss:ebp+8]  ; ax = mascara de 16 bits
        out	0A1h,al
        pop     ebp
        retn

_read_msw:
       smsw    ax		; Obtiene la Machine Status Word
       retn

_lidt:				; Carga el IDTR
        push    ebp
        mov     ebp, esp
        push    ebx
        mov     ebx, [ss: ebp + 6] ; ds:bx = puntero a IDTR 
	rol	ebx,16		    	
	lidt    [ds: ebx]          ; carga IDTR_lidt (&idtr);	
        pop     ebx
        pop     ebp
        retn


; Debug para el BOCHS, detiene la ejecució
; Para continuar colocar en el BOCHSDBG: set $eax=0

_debug:
        push    bp
        mov     bp, sp
        push	ax
vuelve:	mov     ax, 1
        cmp	ax, 0
	jne	vuelve
	pop	ax
	pop     bp
        retn
