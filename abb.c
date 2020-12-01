#include "abb.h"
#include <stdlib.h>

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
	abb_destruir_dato_t destruir;
};

struct abb_iter {
	size_t cosasfeastetocaavosgabo;
};


/* ******************************************************************
 *                  		PRIMITIVAS ABB
 * *****************************************************************/


abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
	abb_t* abb = malloc(sizeof(abb_t));
	if(!abb)
		return false;

	abb->raiz = NULL;
	abb->cmp = cmp;
	abb->destruir = destruir_dato;
	abb->cant = 0;
	return abb;
}

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

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){
	return;
}


/* ******************************************************************
 *					PRIMITIVAS ITERADOR EXTERNO
 * *****************************************************************/

abb_iter_t *abb_iter_in_crear(const abb_t *arbol){
	abb_iter_t* iter;
	return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter){
	return false;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){
	return NULL;
}

bool abb_iter_in_al_final(const abb_iter_t *iter){
	return false;
}

void abb_iter_in_destruir(abb_iter_t* iter){
	return;
}

