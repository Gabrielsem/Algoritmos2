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

// Pre: -
// Post: Crea un nodo con el dato y nodo siguiente indicado.
// Devuelve NULL si falló la creación, sino un puntero al nodo.
static nodo_t* crear_nodo(void* dato, nodo_t* siguiente){
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if(nodo == NULL)
		return NULL;

	nodo->dato = dato;
	nodo->siguiente = siguiente;
	return nodo;
}

// Pre: Lista ya creada, anterior y siguiente son los nodos entre los cuales
// se quiere insertar el nuevo nodo.
// Post: Crea un nuevo nodo con el dato indicado y lo inserta en la lista entre
// los nodos anterior y siguiente, devolviendo true. Devuelve false si falla.
static bool insertar_nodo(lista_t* lista, void* dato, nodo_t* anterior, nodo_t* siguiente){

	nodo_t* nodo = crear_nodo(dato, siguiente);
	if(!nodo)
		return false;

	if(anterior)
		anterior->siguiente = nodo;
	else
		lista->primero = nodo;

	if((!lista->ultimo) || (!siguiente))
		lista->ultimo = nodo;

	(lista->largo)++;
	return true;
}

// Pre: Lista ya creada, anterior es el nodo anterior al que se va a borrar
// y borrado es el nodo a borrar.
// Post: Borra el nodo de la lista devolviendo su dato. 
// Devuelve NULL si no existe el nodo.
static void* borrar_nodo(lista_t* lista, nodo_t* anterior, nodo_t* borrado){

	if(!borrado)
		return NULL;

	if(anterior)
		anterior->siguiente = borrado->siguiente;
	else
		lista->primero = borrado->siguiente;

	if(!lista->primero)
		lista->ultimo = NULL;
	else if(lista->ultimo == borrado)
		lista->ultimo = anterior;

	void* dato = borrado->dato;
	free(borrado);
	(lista->largo)--;
	return dato;
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
	return insertar_nodo(lista, dato, NULL, lista->primero);
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){
	return insertar_nodo(lista, dato, lista->ultimo, NULL);
}

void *lista_borrar_primero(lista_t *lista){
	return borrar_nodo(lista, NULL, lista->primero);
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

	while(actual && visitar && visitar(actual->dato, extra))
		actual = actual->siguiente;
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

	if(insertar_nodo(iter->lista, dato, iter->anterior, iter->actual)){
		iter->actual = iter->anterior ? iter->anterior->siguiente : iter->lista->primero;
		return true;
	}
	return false;
}

void *lista_iter_borrar(lista_iter_t *iter){

	void* dato = borrar_nodo(iter->lista, iter->anterior, iter->actual);
	iter->actual = iter->anterior ? iter->anterior->siguiente : iter->lista->primero;
	return dato;
}
