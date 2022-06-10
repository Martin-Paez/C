
/* Para poder usar arrays al estilo : typo nombre[cantidad]  
 * o  typo nombre[] = "bla bla".
 * Esto es necesario si los arrays estan dentro de funciones
 */
/* Esta funcion se supone temrmina la funcion si se produjo un
 * stackoverflow. En la realidad en esta implementacion no hace nada
 */ 
void __stack_chk_fail() {
  return;
}