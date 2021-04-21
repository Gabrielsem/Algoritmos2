#include "cola.h"
#include "testing.h"
#include "pila.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define CANTIDAD_VOLUMEN 5000
#define CANTIDAD_DESTRUIR 10

static void prueba_cola_vacia() {

	//Se crea una cola
	cola_t *cola = cola_crear();
	print_test("Creando cola para probar cola_esta_vacia()", cola != NULL);
    if (cola == NULL) {
		printf("No se pudo crear cola para probar cola_esta_vacia()\n");
		return;
	}

 	//Pruebo que una cola nueva esté vacía
	print_test("Cola nueva está vacía", cola_esta_vacia(cola));

	//Pruebo que una cola con elementos no esté vacía
	cola_encolar(cola,(void*) 1);
	print_test("Cola con 1 elemento no está vacía", !cola_esta_vacia(cola));
	cola_encolar(cola,(void*) 2);
	print_test("Cola con 2 elementos no está vacía", !cola_esta_vacia(cola));
	cola_desencolar(cola);
	cola_desencolar(cola);

	//Pruebo que una cola sin elementos esté vacía
	print_test("Cola sin elementos está vacía", cola_esta_vacia(cola));
    
	cola_destruir(cola, NULL);
}

//Pruebas con CANTIDAD_VOLUMEN elementos
static void pruebas_volumen(cola_t* cola) {

	bool pudo_encolar = true;
	bool mantuvo_orden = false;

	//Encolo CANTIDAD_VOLUMEN elementos
	for (size_t i = 0; i < CANTIDAD_VOLUMEN; i++) {
		if( !cola_encolar(cola, (void*) i) )
			pudo_encolar = false;
	}

	//Desapilo los elementos y verifico que sea en el orden correcto
	if (pudo_encolar){
		mantuvo_orden = true;

		for (size_t i = 0; i < CANTIDAD_VOLUMEN; i++) {
			mantuvo_orden = (cola_desencolar(cola) == (void*) i);
		}
	} 

	print_test("Prueba de volumen: Encolar elementos", pudo_encolar);
	print_test("Prueba de volumen: Mantuvo orden al desencolar los elementos", mantuvo_orden);
	print_test("Prueba de volumen: Devuelve NULL después de desencolar todos los elementos", cola_desencolar(cola) == NULL);
}

//Pruebas para las funciones cola_encolar y cola_desencolar
static void prueba_cola_encolar_desencolar() {

	//Se crea una cola
	cola_t *cola = cola_crear();
	print_test("Creando cola para probar cola_encolar() y cola_desencolar()", cola != NULL);
	if (cola == NULL) {
		printf("No se pudo crear cola para probar cola_encolar() y cola_desencolar()\n");
		return;
	}

	//Pruebo que desencolar una cola nueva devuelva NULL
	print_test("Desencolar cola nueva devuelve NULL", cola_desencolar(cola) == NULL);

	//Pruebo que se pueda encolar y desencolar el mismo elemento
	print_test("Se puede encolar un elemento", cola_encolar(cola,(void*) 1));
	print_test("Se desencola el mismo elemento", cola_desencolar(cola) == (void*) 1);

	//Pruebo que se puedan encolar 2 elementos y al desencolar mantenga el orden
	print_test("Se puede encolar 2 elementos", cola_encolar(cola,(void*) 1) && cola_encolar(cola,(void*) 2) );
	print_test("Desencolar en cola de 2 elementos devuelve el primer elemento y luego el segundo", 
		(cola_desencolar(cola) == (void*) 1) && (cola_desencolar(cola) == (void*) 2) );

	//Pruebo que desencolar una cola vacía devuelva NULL
	print_test("Desencolar cola vacía devuelve NULL", cola_desencolar(cola) == NULL);

	pruebas_volumen(cola);
	
	cola_destruir(cola, NULL);
}

static void pruebas_cola_ver_primero() {

	//Se crea una cola
	cola_t *cola = cola_crear();
	print_test("Creando cola para probar cola_ver_primero()", cola != NULL);
	if (cola == NULL) {
		printf("No se pudo crear cola para probar cola_ver_primero()\n");
		return;
	}

	//Pruebo que ver el primero en una cola nueva devuelva NULL
	print_test("Ver tope en cola nueva da NULL", cola_ver_primero(cola) == NULL);

	//Pruebo que ver el primero en una cola siempre devuelva el primer elemento encolado
	cola_encolar(cola,(void*) 1);
	print_test("Ver primero en cola con 1 elemento devuelve ese elemento", cola_ver_primero(cola) == (void*) 1);
	cola_encolar(cola,(void*) 2);
	print_test("Ver tope en cola con 2 elementos devuelve el primer elemento", cola_ver_primero(cola) == (void*) 1);
	cola_desencolar(cola);
	cola_desencolar(cola);

	//Pruebo que  el primero en una cola vacía devuelva NULL
	print_test("Ver primero en cola vacía da NULL", cola_ver_primero(cola) == NULL);

	cola_destruir(cola, NULL);
}

//Pre: i apunta a un size_t
//Post: Duplica el size_t apuntado
void duplicar(void* i) {
	*((size_t*) i) *= 2;
}

static void pruebas_cola_destruir() {

	//Se crean dos colas para las pruebas
	cola_t *cola_funcion = cola_crear();
	cola_t *cola_null = cola_crear();
	print_test("Creando colas para probar cola_destruir()", cola_funcion && cola_null);
	if (!cola_funcion || !cola_null) {
		if (cola_funcion) cola_destruir(cola_funcion, NULL);
		if (cola_null) cola_destruir(cola_null, NULL);
		printf("No se pudo crear colas para probar cola_destruir()\n");
		return;
	}
	
	//Encolo punteros a elementos de vectores en ambas colas e inicializo vectores
	size_t vector_null[CANTIDAD_DESTRUIR];
	size_t vector_funcion[CANTIDAD_DESTRUIR];
	for (size_t i = 0; i < CANTIDAD_DESTRUIR; i++) {
		vector_null[i] = i;
		vector_funcion[i] = i;
		if (!cola_encolar(cola_funcion, vector_funcion+i) || !cola_encolar(cola_null, vector_null+i)) {
			printf("No se pudo encolar elementos para probar cola_destruir()\n");
			return;
		}
	}

	//Verifico que cola_destruir pasando NULL no modifique elementos
	cola_destruir(cola_null,NULL);
	bool no_modifico = true;
	for (size_t i = 0; i < CANTIDAD_DESTRUIR; i++)
		no_modifico = (vector_null[i] == i);
	print_test("No se modifican elementos al destruir pasando NULL", no_modifico);

	//Verifico que cola_destruir aplique la función que se le pasa a todos los elementos
	cola_destruir(cola_funcion, duplicar);
	bool aplico_funcion = true;
	size_t elemento;
	for (size_t i = 0; i < CANTIDAD_DESTRUIR; i++) {
		elemento = i;
		duplicar(&elemento);
		aplico_funcion = (vector_funcion[i] == elemento);
	}
	print_test("Se aplica la función pasada al destuir cola", aplico_funcion);

}

static void pruebas_cola_destruir_free() {

	//Se crea una cola para las pruebas
	cola_t *cola_free = cola_crear();
	print_test("Creando cola para probar cola_destruir() con memoria dinámica", cola_free);
	if (!cola_free) {
		printf("No se pudo crear cola para probar cola_destruir()\n");
		return;
	}
	
	//Se hace malloc de 2 caracteres, se encolan y luego se pasa free al destruir la cola
	char* caracter_1 = malloc(sizeof(char));
	if (!caracter_1) {
		printf("No se pudo pedir memoria para probar cola_destruir()\n");
		cola_destruir(cola_free, NULL);
		return;
	}
	char* caracter_2 = malloc(sizeof(char));
	if (!caracter_2) {
		printf("No se pudo pedir memoria para probar cola_destruir()\n");
		free(caracter_1);
		cola_destruir(cola_free, NULL);
		return;
	}

	if (!cola_encolar(cola_free, (void*) caracter_1) || !cola_encolar(cola_free, (void*) caracter_2)) {
		printf("No se pudo encolar elementos para probar cola_destruir()\n");
		free(caracter_1);
		free(caracter_2);
		cola_destruir(cola_free, NULL);
		return;
	}

	cola_destruir(cola_free, free);
	print_test("Destruir cola con 2 elementos pasandole free (ver valgrind)", true);
}

static void pila_destruir_wr(void* pila) {
	pila_destruir((pila_t*) pila);
}

static void pruebas_cola_destruir_pila() {
	//Se crea una cola para las pruebas
	cola_t *cola_pila = cola_crear();
	print_test("Creando cola para probar cola_destruir() con pilas", cola_pila);
	if (!cola_pila) {
		printf("No se pudo crear cola para probar cola_destruir()\n");
		return;
	}

	//Se hace crean 2 pilas, se apila un elemento en una de ellas,
	//se encolan y luego se pasa destruir_pila al destruir la cola
	pila_t* pila_1 = pila_crear();
	if (!pila_1) {
		printf("Error creando pilas para probar cola_destruir()\n");
		cola_destruir(cola_pila, NULL);
		return;
	}
	pila_t* pila_2 = pila_crear();
	if (!pila_1) {
		printf("Error creando pilas para probar cola_destruir()\n");
		cola_destruir(cola_pila, NULL);
		pila_destruir(pila_1);
		return;
	}
	if (!pila_apilar(pila_1, (void*) 123)) {
		printf("Error apilando un elemento para probar cola_destruir()\n");
		cola_destruir(cola_pila, NULL);
		pila_destruir(pila_1);
		pila_destruir(pila_2);
		return;
	}

	if(!cola_encolar(cola_pila, (void*) pila_1) || !cola_encolar(cola_pila, (void*) pila_2)) {
		printf("No se pudo encolar pilas para probar cola_destruir()\n");
		pila_destruir(pila_1);
		pila_destruir(pila_2);
		cola_destruir(cola_pila, NULL);
		return;
	}

	cola_destruir(cola_pila, pila_destruir_wr);
	print_test("Destruir cola con 2 pilas pasandole pila_destruir (ver valgrind)", true);
}

static void pruebas_cola_null(){
	//Se crea una cola
	cola_t *cola = cola_crear();
	print_test("Creando cola para probar encolar NULL", cola != NULL);
	if(cola == NULL) {
		printf("No se pudo crear cola para probar encolar NULL\n");
		return;
	}

	//Pruebo que encolar y desencolar NULL sea válido
	print_test("Se puede encolar NULL", cola_encolar(cola, NULL));

	//Pruebo que cola con NULL apilado no este vacía
	print_test("Cola con NULL no esta vacía", !cola_esta_vacia(cola));

	//Pruebo que primero de cola con NULL apilado sea NULL
	print_test("Cola con NULL no esta vacía", !cola_ver_primero(cola));

	//Pruebo que desencolar NULL sea válido
	print_test("Se puede desencolar NULL", cola_desencolar(cola) == NULL);

	cola_destruir(cola, NULL);
}


void pruebas_cola_estudiante() {
	printf("\n-PRUEBAS cola_esta_vacia()-\n");
	prueba_cola_vacia();
	printf("\n-PRUEBAS cola_encolar() y cola_desencolar()-\n");
	prueba_cola_encolar_desencolar();
	printf("\n-PRUEBAS cola_ver_primero()-\n");
	pruebas_cola_ver_primero();
	printf("\n-PRUEBAS cola_destruir()-\n");
	pruebas_cola_destruir();
	pruebas_cola_destruir_free();
	pruebas_cola_destruir_pila();
	printf("\n-PRUEBAS encolando NULL-\n");
	pruebas_cola_null();
	printf("\n-ERRORES TOTALES: %i-\n",failure_count());
}

#ifndef CORRECTOR 

int main(void) {
    pruebas_cola_estudiante();
    return failure_count() > 0; 
}

#endif
