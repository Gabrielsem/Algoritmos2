#include "abb.h"
#include <stdlib.h>
#include "pila.h"

/* ******************************************************************
 *                  DEFINICIÓN DE LAS ESTRUCTURAS
 * *****************************************************************/

typedef struct nodo {
	struct nodo* izq;
	struct nodo* der;
	char* clave;
	void* dato;
} nodo_t;

struct abb {
	nodo_t* raiz;
	size_t cant;
	abb_comparar_clave_t cmp;
	abb_destruir_dato_t destruir_dato;
};

struct abb_iter {
	pila_t* pila;
};

/* ******************************************************************
 *                  	   FUNCIONES INTERNAS
 * *****************************************************************/
void abb_in_order_rec(nodo_t* nodo, bool visitar(const char *, void *, void *), void *extra){
	if(!nodo) return;
	abb_in_order_rec(nodo->izq,visitar,extra);
	if(visitar) visitar(nodo->clave,nodo->dato,extra);
	abb_in_order_rec(nodo->der,visitar,extra);
}
//apila el nodo recibido y todos sus hijos izq.
void apilar_izq(pila_t* pila, nodo_t* nodo){
	while(nodo) {
		pila_apilar(pila, nodo);
		if(nodo->izq) nodo = nodo->izq;
	}
}
/* ... */

/* ******************************************************************
 *                  		PRIMITIVAS ABB
 * *****************************************************************/


/*
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
	abb_t* abb;
	return abb;
}
*/
bool abb_guardar(abb_t *arbol, const char *clave, void *dato){
	return false;
}

void *abb_borrar(abb_t *arbol, const char *clave){
	return NULL;
}

void *abb_obtener(const abb_t *arbol, const char *clave){
	return NULL;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){
	return false;
}

size_t abb_cantidad(abb_t *arbol){
	return 0;
}

void abb_destruir(abb_t *arbol){
	return;
}



/* ******************************************************************
 *					PRIMITIVAS ITERADOR INTERNO
 * *****************************************************************/



void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
	abb_in_order_rec(arbol->raiz, visitar, extra);
}

/* ******************************************************************
 *					PRIMITIVAS ITERADOR EXTERNO
 * *****************************************************************/

abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
	abb_iter_t* iter = malloc(sizeof(abb_iter_t));
	if(!iter) return NULL;
	iter->pila = pila_crear();
	if(!iter->pila) {
		free(iter);
		return NULL;
	}
	apilar_izq(iter->pila, arbol->raiz);
	return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter) {
	nodo_t* der = ((nodo_t*)pila_desapilar(iter->pila))->der;
	if(der) apilar_izq(iter->pila, der);
	if(!der && abb_iter_in_al_final(iter)) return false;
	return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
	return ((nodo_t*)pila_ver_tope(iter->pila))->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter) {
	return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter) {
	free(iter->pila);
	free(iter);
}
