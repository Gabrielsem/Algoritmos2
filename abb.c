#include "abb.h"
#include <stdlib.h>
#include "pila.h"
#include <string.h>
#define _POSIX_C_SOURCE 200809L

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

/* Crea un nodo con la clave y el dato indicados.
   Este nodo se guarda con una copia de la clave
   recibida.
   Devuelve NULL si falla.
 */
nodo_t* crear_nodo(const char* clave, void* dato) {
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if(!nodo)
		return NULL;

	nodo->clave = strdup(clave);
	if(!nodo->clave) {
		free(nodo);
		return NULL;
	}

	nodo->dato = dato;
	nodo->izq = NULL;
	nodo->der = NULL;
	return nodo;
}

/* Busca el lugar donde va un nodo con la clave indicada.
   Devuelve un puntero al lugar donde almacena su puntero el nodo padre,
   o donde lo almacenaria su posible padre si el elemento no se encuentra.
   (Es decir, devuelve &(padre->der) o &(padre->izq), segun corresponda).
   No modifica ningun nodo.

   Recibe un puntero al lugar donde se guarda el puntero a la raiz,
   la clave a buscar y la función de comparación.
 */
nodo_t** buscar_nodo(nodo_t** nodo, const char* clave, abb_comparar_clave_t cmp) {
	if(!*nodo)
		return nodo;

	int comparacion = cmp((*nodo)->clave, clave);

	if(comparacion == 0)
		return nodo;

	if(comparacion < 0)
		return buscar_nodo(&((*nodo)->der), clave, cmp);
	return buscar_nodo(&((*nodo)->izq), clave, cmp);
}

/* Busca el lugar donde esta el puntero al mayor nodo del sub-árbol del nodo recibido.
   Devuelve un puntero al lugar donde lo almacena su padre.

   Recibe un puntero al lugar donde esta almacenado el nodo raíz del sub-árbol.
   Pre: el nodo recibido no es NULL. (*nodo != NULL)
 */
nodo_t** buscar_mayor(nodo_t** nodo) {
	if(!(*nodo)->der)
		return nodo;

	return buscar_mayor(&(*nodo)->der);
}


/* Destruye todo el sub-árbol del nodo recibido, liberando la
   memoria de las claves y aplicando la función destruir_dato
   a cada dato si esta no es NULL.
 */
void destruir_arbol_aux(nodo_t* nodo, abb_destruir_dato_t destruir_dato) {
	if(!nodo)
		return;
	
	destruir_arbol_aux(nodo->izq, destruir_dato);
	destruir_arbol_aux(nodo->der, destruir_dato);

	free(nodo->clave);
	if(destruir_dato)
		destruir_dato(nodo->dato);

	free(nodo);
}

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

/* ******************************************************************
 *                  		PRIMITIVAS ABB
 * *****************************************************************/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
	abb_t* abb = malloc(sizeof(abb_t));
	if(!abb)
		return false;

	abb->raiz = NULL;
	abb->cmp = cmp;
	abb->destruir_dato = destruir_dato;
	abb->cant = 0;
	return abb;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
	nodo_t** nodo = buscar_nodo(&(arbol->raiz), clave, arbol->cmp);

	if(*nodo) {
		if(arbol->destruir_dato)
			arbol->destruir_dato((*nodo)->dato);

		(*nodo)->dato = dato;
	} else {
		*nodo = crear_nodo(clave, dato);
		if(!*nodo)
			return false;

		arbol->cant++;
	}

	return true;
}

void *abb_borrar(abb_t *arbol, const char *clave) {
	nodo_t** nodo = buscar_nodo(&(arbol->raiz), clave, arbol->cmp);
	if(!*nodo)
		return NULL;

	nodo_t* nuevo_hijo = NULL;
	if((*nodo)->der && (*nodo)->izq) {
		nodo_t** pos_reemplazante = buscar_mayor(&(*nodo)->izq);
		nuevo_hijo = *pos_reemplazante;
		*pos_reemplazante = nuevo_hijo->izq;
		nuevo_hijo->der = (*nodo)->der;
		nuevo_hijo->izq = (*nodo)->izq == nuevo_hijo ? NULL : (*nodo)->izq;
	} else if ((*nodo)->der) {
		nuevo_hijo = (*nodo)->der;
	} else {
		nuevo_hijo = (*nodo)->izq;
	}

	free((*nodo)->clave);
	void* dato = (*nodo)->dato;
	free(*nodo);
	*nodo = nuevo_hijo;
	arbol->cant--;
	return dato;
}

void *abb_obtener(const abb_t *arbol, const char *clave) {
	nodo_t** nodo = buscar_nodo((nodo_t**) &(arbol->raiz), clave, arbol->cmp);
	if(!*nodo)
		return NULL;
	return (*nodo)->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
	return *buscar_nodo((nodo_t**) &(arbol->raiz), clave, arbol->cmp);
}

size_t abb_cantidad(abb_t *arbol) {
	return arbol->cant;
}

void abb_destruir(abb_t *arbol) {
	destruir_arbol_aux(arbol->raiz, arbol->destruir_dato);
	free(arbol);
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
