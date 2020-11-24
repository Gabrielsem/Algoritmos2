#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ******************************************************************
 *                  DEFINICIÓN DE LAS ESTRUCTURAS
 * *****************************************************************/

#define CAPACIDAD_INICIAL 10
#define PROP_AGRANDAR 0.7
#define PROP_ACHICAR 0.15
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
size_t hash_func(const char *str, size_t cap){
    size_t hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + (size_t) c; /* hash * 33 + c */

    return hash % cap;
}

// Inicializa esa cantidad de elementos del vector elementos,
// poniendo su estado en VACIO.
void inicializar_elementos(elem_t* elementos, size_t cant){
	for(int i = 0; i < cant; i++){
		elementos[i].estado = VACIO;
	}
}

// Devuelve una nueva capacidad ideal para ese hash de acuerdo a su carga,
// según la proporción de la cantidad de elementos con la capacidad del hash:
// - Si es menor a PROP_ACHICAR, devuelve la capacidad actual
//   del hash dividido FACTOR_REDIM.
// - Si es mayor a PROP_AGRANDAR, devuelve la capacidad actual
//   del hash multiplicado por FACTOR_REDIM.
// - Sino, devuelve la capacidad actual del hash.
size_t nueva_cap(const hash_t* hash){
	size_t cap = hash->cap;
	float proporcion = ((float) hash->cant)/((float) hash->cap);

	if(proporcion < PROP_ACHICAR){
		cap = hash->cap/FACTOR_REDIM;
		if(cap < CAPACIDAD_INICIAL)
			cap = CAPACIDAD_INICIAL;
	}

	if(proporcion > PROP_AGRANDAR){
		cap = hash->cap * FACTOR_REDIM;
	}

	return cap;
}

// Redimensiona el hash si es necesario.
// Devuelve verdadero si lo logró o no era necesario,
// sino falso.
bool redim_hash(hash_t* hash){
	size_t cap = nueva_cap(hash);
	if(cap == hash->cap)
		return true;

	elem_t* nuevos_elem = malloc(sizeof(elem_t)*cap);
	if(!nuevos_elem)
		return false;

	inicializar_elementos(nuevos_elem, cap);

	for(int i = 0; i < hash->cap; i++){
		if(hash->elementos[i].estado == OCUPADO){
			size_t pos_ini = hash_func(hash->elementos[i].clave, hash->cap);
			size_t j = pos_ini;

			do {
				if(nuevos_elem[j].estado != OCUPADO){
					break;
				}
				j++;
				if(j == cap)
					j = 0;
			} while (j != pos_ini);

			nuevos_elem[j] = hash->elementos[i];
		}
	}

	free(hash->elementos);
	hash->elementos = nuevos_elem;
	hash->cap = cap;
	return true;
}

/* ******************************************************************
 *                         PRIMITIVAS HASH
 * *****************************************************************/

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash = malloc(sizeof(hash_t));
	if(!hash)
		return NULL;

	hash->elementos = malloc(sizeof(elem_t)*CAPACIDAD_INICIAL);
	if(!hash->elementos){
		free(hash);
		return NULL;
	}

	inicializar_elementos(hash->elementos, CAPACIDAD_INICIAL);
	hash->cap = CAPACIDAD_INICIAL;
	hash->cant = 0;
	hash->funcion_destruir = destruir_dato;
	return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
	if (!redim_hash(hash)) return false;

	size_t pos_ini = hash_func(clave, hash->cap);
	size_t pos = pos_ini;
	size_t pos_borrado;
	bool hay_borrado = false;

	char* clave_copia;

	while(hash->elementos[pos].estado != VACIO ||
	(hash->elementos[pos].estado == OCUPADO && strcmp(hash->elementos[pos].clave,clave) == 0)){
		
		if(!hay_borrado && hash->elementos[pos].estado == BORRADO){
			hay_borrado = true;
			pos_borrado = pos;		
		}

		pos++;
		if(pos == pos_ini) //Si pasa esto no hay vacios, todos borrados y no estaba el elemento
			break;
		if(pos == hash->cap)
			pos = 0;
	}

	if(hash->elementos[pos].estado == OCUPADO){
		hash->funcion_destruir(hash->elementos[pos].dato);
		clave_copia = hash->elementos[pos].clave;
		hash->elementos[pos].estado = BORRADO;
	} else {
		clave_copia = strdup(clave);
		if(!clave_copia) return false;
		hash->cant++;
	}

	if(hay_borrado)
		pos = pos_borrado;

	hash->elementos[pos].dato = dato;
	hash->elementos[pos].clave = clave_copia;
	hash->elementos[pos].estado = OCUPADO;

	return true;
}

void *hash_borrar(hash_t *hash, const char *clave){
	redim_hash(hash);

	size_t pos_ini = hash_func(clave, hash->cap);
	size_t i = pos_ini;
	elem_t* elementos = hash->elementos;
	while(elementos[i].estado != VACIO) {
		if(elementos[i].estado == OCUPADO && strcmp(elementos[i].clave, clave) == 0){
			elementos[i].estado = BORRADO;
			hash->cant--;
			free(elementos[i].clave);
			return elementos[i].dato;
		}

		i++;
		if(i == pos_ini)
			break;
		if(i == hash->cap)
			i = 0;
	}

	return NULL;
}

void *hash_obtener(const hash_t *hash, const char *clave){
	size_t pos_ini = hash_func(clave, hash->cap);
	size_t i = pos_ini;
	elem_t* elementos = hash->elementos;
	while(elementos[i].estado != VACIO) {
		if((elementos[i].estado == OCUPADO && strcmp(elementos[i].clave, clave) == 0)){
			return elementos[i].dato;
		}

		i++;
		if(i == hash->cap)
			i = 0;
		if(i == pos_ini)
			break;
	}

	return NULL;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
	size_t pos_ini = hash_func(clave,hash->cap);
	size_t i = pos_ini;
	elem_t* elementos = hash->elementos;
	while(elementos[i].estado != VACIO) {
		if(elementos[i].estado == OCUPADO && strcmp(elementos[i].clave, clave) == 0) return true;

		i++;
		if(i == hash->cap)
			i = 0;
		if(i == pos_ini)
			break;
	} while (elementos[i].estado != VACIO);

	return false;
}

size_t hash_cantidad(const hash_t *hash){
	return hash->cant;
}

void hash_destruir(hash_t *hash){
	elem_t* elementos = hash->elementos;
	if(hash->funcion_destruir != NULL){
		for(size_t i = 0; i < hash->cap; i++){
			if(elementos[i].estado != OCUPADO) continue;
			hash->funcion_destruir(elementos[i].dato);
			free(elementos[i].clave);
		}
	}
	free(hash->elementos);
	free(hash);
}

/* ******************************************************************
 *                       PRIMITIVAS ITERADOR                        *
 * *****************************************************************/

bool hash_iter_avanzar(hash_iter_t *iter){
	if(iter->pos == iter->hash->cap){
		return false;
	}

	do {
		iter->pos++;
	} while((iter->pos < iter->hash->cap) && (iter->hash->elementos[iter->pos].estado != OCUPADO));

	return true;
}

hash_iter_t *hash_iter_crear(const hash_t *hash){
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if(!iter)
		return NULL;

	iter->pos = 0;
	iter->hash = hash;

	hash_iter_avanzar(iter);

	return iter;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
	if(iter->pos == iter->hash->cap)
		return NULL;
	
	return iter->hash->elementos[iter->pos].clave;
}

bool hash_iter_al_final(const hash_iter_t *iter){
	return iter->pos == iter->hash->cap;
}

void hash_iter_destruir(hash_iter_t *iter){
	free(iter);
}
