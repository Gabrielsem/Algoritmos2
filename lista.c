#include "lista.h"
#include <stdlib.h>

typedef struct nodo {
	void* dato;
	struct nodo* siguiente;
} nodo_t;

struct lista {
	nodo_t* primero;
	nodo_t* ultimo;
	size_t largo;
};

struct lista_iter {
	lista_t* lista;
	nodo_t* actual;
	nodo_t* anterior;
};

/* ******************************************************************
 *                    FUNCIONES USO INTERNO
 * *****************************************************************/


//Pre: -
//Post:	Crea un nodo con el dato indicado, inicializando el siguiente en NULL.
//		Devuelve NULL si falló la creación, sino un puntero al nodo.
static nodo_t* crear_nodo(void* dato){
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if(nodo == NULL)
		return NULL;

	nodo->dato = dato;
	nodo->siguiente = NULL;
	return nodo;
}

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

lista_t* lista_crear(){

	lista_t* lista = malloc(sizeof(lista_t));
	if(!lista)
		return NULL;

	lista->primero = NULL;
	lista->ultimo = NULL;
	lista->largo = 0;
	return lista;
}

bool lista_esta_vacia(const lista_t* lista){
	return(lista->largo == 0);
}

bool lista_insertar_primero(lista_t *lista, void *dato){
	
	nodo_t* nodo = crear_nodo(dato);
	if(!nodo)
		return false;

	nodo->siguiente = lista->primero;
	lista->primero = nodo;
	(lista->largo)++;

	if(!lista->ultimo)
		lista->ultimo = nodo;

	return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){

	nodo_t* nodo = crear_nodo(dato);
	if(!nodo)
		return false;

	lista->ultimo->siguiente = nodo;
	lista->ultimo = nodo;
	(lista->largo)++;
	return true;
}

void *lista_borrar_primero(lista_t *lista){

	if(lista->primero == NULL)
		return NULL;

	nodo_t primero = *(lista->primero);
	free(lista->primero);
	lista->primero = primero.siguiente;
	(lista->largo)--;

	if(!lista->primero)
		lista->ultimo = NULL;

	return primero.dato;
}

void *lista_ver_primero(const lista_t *lista){

	if(lista->primero == NULL)
		return NULL;

	return lista->primero->dato;
}

void *lista_ver_ultimo(const lista_t* lista){

	if(lista->ultimo == NULL)
		return NULL;

	return lista->ultimo->dato;
}

size_t lista_largo(const lista_t *lista){
	return lista->largo;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)){
	
	nodo_t* siguiente;
	while(lista->primero){

		if(destruir_dato)
			destruir_dato(lista->primero->dato);

		siguiente = lista->primero->siguiente;
		free(lista->primero);
		lista->primero = siguiente;
	}

	free(lista);
}

/* ******************************************************************
 *               PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){

	nodo_t* actual = lista->primero;
	while(actual){

		if(visitar && (!visitar(actual->dato,extra)))
			return;

		actual = actual->siguiente;
	}
}

/* ******************************************************************
 *               PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

lista_iter_t *lista_iter_crear(lista_t *lista){

	lista_iter_t* iter = malloc(sizeof(lista_iter_t));
	if(!iter)
		return NULL;

	iter->lista = lista;
	iter->actual = lista->primero;
	iter->anterior = NULL;
	return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter){

	if(!iter->actual)
		return false;

	iter->anterior = iter->actual;
	iter->actual = iter->actual->siguiente;
	return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){

	if(!iter->actual)
		return NULL;
	return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter){
	return (!iter->actual);
}

void lista_iter_destruir(lista_iter_t *iter){
	free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
	
	nodo_t* nodo = crear_nodo(dato);
	if(!nodo)
		return false;

	if(iter->anterior)
		iter->anterior->siguiente = nodo;
	else
		iter->lista->primero = nodo;

	if(!iter->lista->ultimo)
		iter->lista->ultimo = nodo;

	nodo->siguiente = iter->actual;
	(iter->lista->largo)++;
	return true;
}

void *lista_iter_borrar(lista_iter_t *iter){

	if(!iter->actual)
		return NULL;

	if(iter->anterior)
		iter->anterior->siguiente = iter->actual->siguiente;
	else
		iter->lista->primero = iter->actual->siguiente;

	if(!iter->lista->primero)
		iter->lista->ultimo = NULL;

	nodo_t nodo = *(iter->actual);
	free(iter->actual);
	iter->actual = nodo.siguiente;
	(iter->lista->largo)--;
	return nodo.dato;
}
