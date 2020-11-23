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
} elem_t;

struct hash {
	elem_t* elementos;
	size_t cap;
	size_t cant;
	hash_destruir_dato_t funcion_destruir;
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
size_t hash_func(char *str, size_t cap){
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
	float proporcion = (float)(hash->cant / hash->cap);

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

bool redim_hash(hash_t* hash){
	size_t cap = nueva_cap(hash);
	if(cap == hash->cap)
		return true;

	//hago cosas
	return true;
}

bool es_valido_guardar(elem_t elemento, char* clave){
	if(elemento.estado == VACIO || elemento.estado == BORRADO) return true;
	if(strcmp(elemento.clave, clave) == 0) return true;
	return false;
}
bool es_elemento_buscado(elem_t elemento, char* clave){
	if(strcmp(elemento.clave, clave) == 0 && elemento.estado == OCUPADO) return true;
	return false;
}

/* ******************************************************************
 *                         PRIMITIVAS HASH
 * *****************************************************************/

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash = malloc(sizeof(hash_t));
	if(!hash)
		return NULL;

	elem_t* elementos = malloc(sizeof(elem_t)*CAPACIDAD_INICIAL);
	if(!elementos){
		free(hash);
		return NULL;
	}

	inicializar_elementos(elementos, CAPACIDAD_INICIAL);
	hash->cap = CAPACIDAD_INICIAL;
	hash->cant = 0;
	hash->funcion_destruir = destruir_dato;
	return hash;
}
/* size_t i = pos_ini;
do {
	i++;
	if(i == cap){
		i = 0;
	}
} while (i != pos_ini);
*/
bool hash_guardar(hash_t *hash, const char *clave, void *dato){
	size_t pos_ini = hash_func(clave,hash->cap);
	size_t i = pos_ini;
	size_t pos_final = hash->cap; //inicializo en cap porque esta fuera del arreglo
	size_t pos_posible = hash->cap;
	elem_t* elementos = hash->elementos;
	do {
		if((strcmp(elementos[i]->clave, clave) == 0 && elementos[i]->estado == OCUPADO) || (elementos[i]->estado == VACIO)){
			pos_final = i;
			break;
		}
		if(elementos[i]->estado == BORRADO && pos_posible == hash->cap){
			pos_posible = i;
		}
		i++;
		if(i == cap){
			i = 0;
		}
	} while (i != pos_ini);

	if(pos_final == hash->cap) return false;

	if(elementos[pos_final]->estado == OCUPADO){
		if(pos_posible != hash->cap){
			elementos[pos_final]->estado = BORRADO;
			if(hash->funcion_destruir != NULL) hash->funcion_destruir(elementos[pos_posible]->dato);
			elementos[pos_posible]->clave = clave;
			elementos[pos_posible]->dato = dato;
			elementos[pos_posible]->estado = OCUPADO;
			hash->cant++;
			return true;
		}
		if(hash->funcion_destruir != NULL) hash->funcion_destruir(elementos[pos_final]->dato);
		elementos[pos_final]->dato = dato;
		return true;
	}
	// en caso de que la pos final sea vacio
	elementos[pos_final]->clave = clave;
	elementos[pos_final]->dato = dato;
	elementos[pos_final]-> estado = OCUPADO;
	hash->cant++;
	return true;
}

void *hash_borrar(hash_t *hash, const char *clave){
	size_t pos_ini = hash_func(clave,hash->cap);
	size_t pos_final = hash->cap;
	size_t i = pos_ini;
	elem_t* elementos = hash->elementos;
	do {
		if((strcmp(elementos[i]->clave, clave) == 0 && elementos[i]->estado == OCUPADO) || (elementos[i]->estado == VACIO)){
			pos_final = i;
			break;
		}
		i++;
		if(i == cap){
			i = 0;
		}
	} while (i != pos_ini);
	if(pos_final == hash->cap) return NULL;
	elementos[pos_final]-> estado = BORRADO;
	hash->cant--;
	return elementos[pos_final]->dato;
}

void *hash_obtener(const hash_t *hash, const char *clave){
	size_t pos_ini = hash_func(clave,hash->cap);
	size_t pos_final = hash->cap;
	size_t i = pos_ini;
	elem_t* elementos = hash->elementos;
	do {
		if((strcmp(elementos[i]->clave, clave) == 0 && elementos[i]->estado == OCUPADO) || (elementos[i]->estado == VACIO)){
			pos_final = i;
			break;
		}
		i++;
		if(i == cap){
			i = 0;
		}
	} while (i != pos_ini);
	if(pos_final == hash->cap) return NULL;
	return elementos[pos_final]->dato;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
	size_t pos_ini = hash_func(clave,hash->cap);
	size_t i = pos_ini;
	elem_t* elementos = hash->elementos;
	do {
		if(strcmp(elementos[i]->clave, clave) == 0 && elementos[i]->estado == OCUPADO) return true;
		if(elementos[i]->estado == VACIO) return false;
		i++;
		if(i == cap){
			i = 0;
		}
	} while (i != pos_ini);
	if(pos_final == hash->cap) return false;
}

size_t hash_cantidad(const hash_t *hash){
	return hash->cant;
}

void hash_destruir(hash_t *hash){
	elem_t* elementos = hash->elementos;
	if(hash->funcion_destruir != NULL){
		for(size_t i = 0, i < hash->cap, i++){
			if(elementos[i]->estado == VACIO) continue;
			hash->funcion_destruir(elementos[i]->dato);
		}
	}
	free(hash->elementos);
	free(hash);
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
