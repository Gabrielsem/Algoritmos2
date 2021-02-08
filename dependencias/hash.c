#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE 1
#define _XOPEN_SOURCE 500
#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ******************************************************************
 *                  DEFINICIÓN DE LAS ESTRUCTURAS
 * *****************************************************************/

#define CAPACIDAD_INICIAL 10
#define PORC_AGRANDAR 70
#define PORC_ACHICAR 15
#define FACTOR_REDIM 2

enum estados {
	VACIO,
	BORRADO,
	OCUPADO
};

typedef struct elemento {
	char* clave;
	void* dato;
	enum estados estado;
} elem_t;

struct hash {
	elem_t* elementos;
	size_t cap;
	size_t cant;
	hash_destruir_dato_t funcion_destruir;
};

struct hash_iter {
	const hash_t* hash;
	size_t pos;
};

typedef bool (*buscar_f)(elem_t elemento, char* extra);

/* ******************************************************************
 *                        FUNCIONES INTERNAS
 * *****************************************************************/

// Función de hash
// (djb2, http://www.cse.yorku.ca/~oz/hash.html)
// Tiene algunas modificaciones para comodidad y
// para que no tire warnings el compilador.
size_t hash_func(const char *str, size_t cap) {
    size_t hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + (size_t) c; /* hash * 33 + c */

    return hash % cap;
}

// Inicializa esa cantidad de elementos del vector elementos,
// poniendo su estado en VACIO.
void inicializar_elementos(elem_t* elementos, size_t cant) {
	for(size_t i = 0; i < cant; i++) {
		elementos[i].estado = VACIO;
	}
}

// Devuelve una nueva capacidad ideal para ese hash de acuerdo a su carga,
// según el porcentaje de la cantidad de elementos ocupados en la capacidad del hash:
// - Si es menor a PORC_ACHICAR, devuelve la capacidad actual
//   del hash dividido FACTOR_REDIM.
// - Si es mayor a PORC_AGRANDAR, devuelve la capacidad actual
//   del hash multiplicado por FACTOR_REDIM.
// - Sino, devuelve la capacidad actual del hash.
size_t nueva_cap(const hash_t* hash) {
	size_t cap = hash->cap;
	size_t porcentaje =  hash->cant * 100 / hash->cap;

	if(porcentaje < PORC_ACHICAR) {
		cap = hash->cap/FACTOR_REDIM;
		if(cap < CAPACIDAD_INICIAL)
			cap = CAPACIDAD_INICIAL;
	}

	if(porcentaje > PORC_AGRANDAR) {
		cap = hash->cap * FACTOR_REDIM;
	}

	return cap;
}

// Redimensiona el hash si es necesario.
// Devuelve verdadero si lo logró o no era necesario,
// sino falso.
bool redim_hash(hash_t* hash) {
	size_t cap = nueva_cap(hash);
	if(cap == hash->cap)
		return true;

	elem_t* nuevos_elem = malloc(sizeof(elem_t) * cap);
	if(!nuevos_elem)
		return false;

	inicializar_elementos(nuevos_elem, cap);

	for(size_t i = 0; i < hash->cap; i++) {
		if(hash->elementos[i].estado == OCUPADO){
			size_t j = hash_func(hash->elementos[i].clave, cap);

			while(nuevos_elem[j].estado != VACIO){
				j++;
				if(j == cap)
					j = 0;
			}

			nuevos_elem[j] = hash->elementos[i];
		}
	}

	free(hash->elementos);
	hash->elementos = nuevos_elem;
	hash->cap = cap;
	return true;
}

//Devuelve true si el elemento tiene esa clave, sino false.
bool misma_clave(elem_t elemento, const char* clave) {
	return elemento.estado == OCUPADO && strcmp(elemento.clave, clave) == 0;
}

// Busca el elemento con esa clave en la tabla de hash.
// Si lo encuentra, devuelve su posición.
// Si no la encuentra, devuelve la posición del primer borrado o vacío.
size_t buscar_elem(elem_t* elementos, size_t cap, const char* clave) {
	size_t pos_ini = hash_func(clave, cap);
	size_t pos = pos_ini;
	size_t pos_borrado;
	bool hay_borrado = false;

	while(elementos[pos].estado != VACIO && !misma_clave(elementos[pos], clave)) {
		
		if(!hay_borrado && elementos[pos].estado == BORRADO) {
			hay_borrado = true;
			pos_borrado = pos;		
		}

		pos++;
		if(pos == pos_ini) //Si pasa esto no hay vacios, todos borrados y no estaba el elemento
			break;
		if(pos == cap)
			pos = 0;
	}
	if(hay_borrado && elementos[pos].estado != OCUPADO)
		pos = pos_borrado;

	return pos;
}

// Busca el primer elemento ocupado a partir de la posicion inicial indicada.
// Devuelve su posicion, o cap si llego al final.
size_t buscar_sig(elem_t* elementos, size_t pos_ini, size_t cap) {
	size_t pos = pos_ini;

	while(pos < cap && elementos[pos].estado != OCUPADO) {
		pos++;
	}

	return pos;
}

/* ******************************************************************
 *                         PRIMITIVAS HASH
 * *****************************************************************/

hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
	hash_t* hash = malloc(sizeof(hash_t));
	if(!hash)
		return NULL;

	hash->elementos = malloc(sizeof(elem_t) * CAPACIDAD_INICIAL);
	if(!hash->elementos) {
		free(hash);
		return NULL;
	}

	inicializar_elementos(hash->elementos, CAPACIDAD_INICIAL);
	hash->cap = CAPACIDAD_INICIAL;
	hash->cant = 0;
	hash->funcion_destruir = destruir_dato;
	return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
	if (!redim_hash(hash)) return false;

	size_t pos = buscar_elem(hash->elementos, hash->cap, clave);

	if(hash->elementos[pos].estado == OCUPADO) {
		if(hash->funcion_destruir)
			hash->funcion_destruir(hash->elementos[pos].dato);

	} else {
		hash->elementos[pos].clave = strdup(clave);
		if(!hash->elementos[pos].clave) return false;
		hash->elementos[pos].estado = OCUPADO;
		hash->cant++;
	}

	hash->elementos[pos].dato = dato;
	return true;
}

void *hash_borrar(hash_t *hash, const char *clave) {
	redim_hash(hash);

	size_t pos = buscar_elem(hash->elementos, hash->cap, clave);

	if(hash->elementos[pos].estado == OCUPADO) {
		hash->elementos[pos].estado = BORRADO;
		hash->cant--;
		free(hash->elementos[pos].clave);
		return hash->elementos[pos].dato;
	}

	return NULL;
}

void *hash_obtener(const hash_t *hash, const char *clave) {
	size_t pos = buscar_elem(hash->elementos, hash->cap, clave);

	if(hash->elementos[pos].estado == OCUPADO) {
		return hash->elementos[pos].dato;
	}

	return NULL;
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
	size_t pos = buscar_elem(hash->elementos, hash->cap, clave);

	return hash->elementos[pos].estado == OCUPADO;
}

size_t hash_cantidad(const hash_t *hash) {
	return hash->cant;
}

void hash_destruir(hash_t *hash) {
	elem_t* elementos = hash->elementos;

	for(size_t i = 0; i < hash->cap; i++) {
		if(elementos[i].estado != OCUPADO) continue;

		if(hash->funcion_destruir)
			hash->funcion_destruir(elementos[i].dato);
		free(elementos[i].clave);
	}

	free(hash->elementos);
	free(hash);
}

/* ******************************************************************
 *                       PRIMITIVAS ITERADOR                        *
 * *****************************************************************/

bool hash_iter_avanzar(hash_iter_t *iter) {
	if(iter->pos == iter->hash->cap)
		return false;

	iter->pos = buscar_sig(iter->hash->elementos, iter->pos + 1, iter->hash->cap);

	return true;
}

hash_iter_t *hash_iter_crear(const hash_t *hash) {
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if(!iter)
		return NULL;

	iter->hash = hash;

	iter->pos = buscar_sig(hash->elementos, 0, hash->cap);

	return iter;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter) {
	if(iter->pos == iter->hash->cap)
		return NULL;
	
	return iter->hash->elementos[iter->pos].clave;
}

bool hash_iter_al_final(const hash_iter_t *iter) {
	return iter->pos == iter->hash->cap;
}

void hash_iter_destruir(hash_iter_t *iter) {
	free(iter);
}
