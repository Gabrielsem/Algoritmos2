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
	hash_destruir_dato_t function_destruir;
};

struct hash_iter {
	hash_t* hash;
	size_t pos;
};

//Tipo de función que recibe buscar_elem(), declarada más abajo
typedef bool (*buscar_f)(elem_t elemento, char* extra);

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

// Recibe un arreglo de elementos con su capacidad, y una posición inicial.
// Se recorre el arreglo a partir de esa posición, volviendo a la posición
// 0 si se llega al final. Se le aplica la función es_elem a cada elemento
// hasta que la función devuelva true, y se devuelve la posición de ese elemento.
// Si la función nunca devuelve true y ya se recorrieron todos los elementos,
// se devuelve la capacidad del vector (como posición inválida).
// pos_ini debe ser menor a cap
size_t buscar_elem(elem_t* elementos, size_t cap, size_t pos_ini , buscar_f es_elem, char* extra){
	size_t i = pos_ini;

	do {
		if(es_elem(elementos[i], extra))
			return i;

		i++;
		if(i == cap)
			i = 0;

	} while (i != pos_ini);

	return cap;
}

// Devuelve verdadero si el elemento esta vacío o borrado.
// El extra es para cumplir con el formato de buscar_f,
// se le puede pasar NULL.
bool esta_libre(elem_t elem, char* extra){
	return (elem.estado == VACIO) || (elem.estado == BORRADO); 
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

// Redimensiona el hash si es necesario.
// Devuelve verdadero si lo logró o no era necesario,
// sino falso.
bool redim_hash(hash_t* hash){
	size_t cap = nueva_cap(hash);
	if(cap == hash->cap)
		return true;

	elem_t* nuevos_elem = malloc(sizeof(elem_t)*nueva_cap);
	if(!nuevos_elem)
		return false;

	inicializar_elementos(nuevos_elem, nueva_cap);

	for(int i = 0; i < hash->capacidad; i++){
		if(hash->elementos[i].estado == OCUPADO){
			size_t pos = hash_func(hash->elementos[i].clave, hash->capacidad);
			pos = buscar_elem(nuevos_elem, nueva_cap, pos, esta_libre, NULL);
			nuevos_elem[pos] = hash->elementos[i];
		}
	}

	free(hash->elementos);
	hash->elementos = nuevos_elem;
	hash->capacidad = nueva_cap;
	return true;
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
