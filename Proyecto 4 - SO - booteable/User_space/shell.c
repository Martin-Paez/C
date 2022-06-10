#include "./C/include/stdioImpl.h"
#include "../Kernel/include/bash.h"

static char command[MAX_COMMAND_LENGTH];

void stringIOtest() {
  printfImpl("Rutina de prueba de las instrucciones scanf y printf \n");
  printfImpl("----------------------------------------------------- \n");
  printfImpl("1) Ingrese una horacion (menos de 90 caracteres) que contenga un numero entero y en vez del numero colocar porcentaje : \n");
  scanfImpl("%s", command);
  int i = 0;
  printfImpl("Ingrese un numero distinto de cero con el cual completar \
	 la frase: \n");
  scanfImpl("%d",&i);
  printfImpl("La frase completa es: \n");
  printfImpl(command, i);
}

void charIOtest() {
  printfImpl("Rutina de prueba de las instrucciones getc y putc \n");
  printfImpl("----------------------------------------------------- \n");
  printfImpl("Ingrese al menos un caracter y presione enter: \n");
  char c = getcImpl(stdinImpl);
  printfImpl("El caracter ingresado fue: \n");
  putcImpl(c, stdoutImpl);
}

static void run(char *command) {
  /* Observacion : podria aprobecharse que strcmp retorna tambien
   * si un string es mayor que el otro para acceder mas rapidamente
   * a la opcion buscada. Sin embargo para el tpe no tiene sentido
   * complicarse con cosas por el estilo
   */
  if ( strcmpImpl(command, "stringIOtest") == 0 )
    stringIOtest();
  else if ( strcmpImpl(command, "charIOtest") == 0 )
    charIOtest();
  else if ( strcmpImpl(command, "cls") == 0 )
    bash("cls");
  else
    printfImpl("Comando invalido \n");
}

int shell(void) {
	bash("cls");
	while (1) {
		int i;	
		for(i=0; i< MAX_COMMAND_LENGTH; i++)
			command[i] = ' ';
		printfImpl(">");
		scanfImpl("%s", command); 
		run( command );
	  }
  	return 0;
}
