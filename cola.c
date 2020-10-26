#include "cola.h"
#include <stdlib.h>

typedef struct nodo {
	void* dato;
	struct nodo* siguiente;
} nodo_t;

struct cola {
	nodo_t* primero;
	nodo_t* ultimo;
	size_t cantidad;
};

cola_t* cola_crear(){

	cola_t* cola = malloc(sizeof(cola));
	if(cola == NULL)
		return NULL;

	cola->primero = NULL;
	cola->ultimo = NULL;
	cola->cantidad = 0;

	return cola;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)){

	nodo_t* nodo_siguiente;
	nodo_t* nodo = cola->primero;
	while(nodo != NULL){

		if(destruir_dato != NULL)
			destruir_dato(nodo->dato);

		nodo_siguiente = nodo->siguiente;
		free(nodo);
		nodo = nodo_siguiente;
	}

	free(cola);
}

bool cola_esta_vacia(const cola_t *cola){
	return (cola->cantidad == 0);
}

bool cola_encolar(cola_t *cola, void *valor){

	nodo_t* nodo = malloc(sizeof(nodo));
	if(nodo == NULL)
		return false;

	nodo->dato = valor;
	nodo->siguiente = NULL;

	if(cola->primero == NULL)
		cola->primero = nodo;
	if(cola->ultimo != NULL)
		cola->ultimo->siguiente = nodo;

	cola->ultimo = nodo;
	(cola->cantidad)++;

	return true;
}

void *cola_ver_primero(const cola_t *cola){
	return( cola->cantidad == 0 ? NULL : cola->primero->dato);
}

void *cola_desencolar(cola_t *cola){

	if(cola->cantidad == 0)
		return NULL;
	
	void* dato = cola->primero->dato;	
	nodo_t* siguiente = cola->primero->siguiente;
	free(cola->primero);
	cola->primero = siguiente;
	(cola->cantidad)--;

	return dato;
}
