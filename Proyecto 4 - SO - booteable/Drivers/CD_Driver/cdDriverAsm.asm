GLOBAL	_out
GLOBAL  _in

SECTION .text

_out:
	pop	edx
	pop	eax

	out	dx, eax
	ret

_in:
	pop	edx
	in	eax, dx
	ret
