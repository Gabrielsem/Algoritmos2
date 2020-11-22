#include "hash.h"
#include <stdlib.h>


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
} elemento_t;

struct hash {
	elemento_t* elementos;
	size_t capacidad;
	size_t cantidad;
	hash_destruir_dato_t function_destruir;
};

struct hash_iter {
	hash_t* hash;
	size_t pos;
};

/* ******************************************************************
 *                        FUNCIONES INTERNAS
 * *****************************************************************/

// Función de hash
// (djb2, http://www.cse.yorku.ca/~oz/hash.html)
// Tiene algunas modificaciones para comodidad y
// para que no tire warnings el compilador.
size_t hash_func(unsigned char *str, size_t capacidad){
    size_t hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + (size_t) c; /* hash * 33 + c */

    return hash % capacidad;
}

// Inicializa esa cantidad de elementos del vector elementos,
// poniendo su estado en VACIO.
void inicializar_elementos(elemento_t* elementos, size_t cantidad){
	for(int i = 0; i < cantidad; i++){
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
size_t nueva_capacidad(const hash_t* hash){
	size_t capacidad = hash->capacidad;
	float proporcion = hash->cantidad / hash->capacidad;

	if(proporcion < PROP_ACHICAR){
		capacidad = hash->capacidad/FACTOR_REDIM;
		if(capacidad < CAPACIDAD_INICIAL)
			capacidad = CAPACIDAD_INICIAL;
	}

	if(proporcion > PROP_AGRANDAR){
		capacidad = hash->capacidad * FACTOR_REDIM;
	}

	return capacidad;
}

bool redim_hash(hash_t* hash){
	size_t nueva_cap = nueva_capacidad(hash);
	if(nueva_cap == hash->capacidad)
		return true;

	//hago cosas
	return true;
}

/* ******************************************************************
 *                         PRIMITIVAS HASH
 * *****************************************************************/

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash = malloc(sizeof(hash_t));
	if(!hash)
		return NULL;

	elemento_t* elementos = malloc(sizeof(elemento_t)*CAPACIDAD_INICIAL);
	if(!elementos){
		free(hash);
		return NULL;
	}

	inicializar_elementos(elementos, CAPACIDAD_INICIAL);
	hash->capacidad = CAPACIDAD_INICIAL;
	hash->cantidad = 0;
	hash->function_destruir = destruir_dato;
	return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){
	return true;
}

void *hash_borrar(hash_t *hash, const char *clave){
	return NULL;
}

void *hash_obtener(const hash_t *hash, const char *clave){
	return NULL;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
	return true;
}

size_t hash_cantidad(const hash_t *hash){
	return 0;
}

void hash_destruir(hash_t *hash){
	return;
}

/* ******************************************************************
 *                       PRIMITIVAS ITERADOR
 * *****************************************************************/

hash_iter_t *hash_iter_crear(const hash_t *hash){
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if(!iter)
		return NULL;

	iter->pos = 0;
	return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter){
	return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
	return NULL;
}

bool hash_iter_al_final(const hash_iter_t *iter){
	return true;
}

void hash_iter_destruir(hash_iter_t *iter){
	return;
}
