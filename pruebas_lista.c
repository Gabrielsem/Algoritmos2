#include "lista.h"
#include "testing.h"
#include <stdio.h>

static void prueba_lista_vacia() {

	//Se crea una lista
	lista_t *lista = lista_crear();
	print_test("Creando lista para probar lista_esta_vacia()", lista != NULL);
    if(lista == NULL){
		printf("No se pudo crear lista para probar lista_esta_vacia()\n");
		return;
	}

 	//Pruebo que una lista nueva esté vacía
	print_test("Lista nueva está vacía", lista_esta_vacia(lista));

	//Pruebo que una lista con elementos no esté vacía
	lista_insertar_primero(lista,(void*) 1);
	print_test("Lista con 1 elemento no está vacía", !lista_esta_vacia(lista));
	lista_insertar_primero(lista,(void*) 2);
	print_test("Lista con 2 elementos no está vacía", !lista_esta_vacia(lista));
	lista_borrar_primero(lista);
	lista_borrar_primero(lista);

	//Pruebo que una lista sin elementos esté vacía
	print_test("Lista sin elementos está vacía", lista_esta_vacia(lista));
    
	lista_destruir(lista, NULL);
}


void pruebas_lista_estudiante() {
	printf("\n-PRUEBAS lista_esta_vacia()-\n");
	prueba_lista_vacia();
	return;
}

#ifndef CORRECTOR 

int main(void) {
    pruebas_lista_estudiante();
    return failure_count() > 0; 
}

#endif
