#include "heap.h"
#include <stdlib.h>

/* ******************************************************************
 *                   DEFINICIÓN DE LAS ESTRUCTURAS
 * *****************************************************************/

struct heap {
	void** datos;
	size_t cap;
	size_t cant;
	cmp_func_t cmp;
};

/* ******************************************************************
 *                  	   FUNCIONES INTERNAS
 * *****************************************************************/

// Pre: pos_hijo es > 0
size_t pos_padre(size_t pos_hijo) {
	return (pos_hijo - 1) / 2;
}

size_t pos_hijo_der(size_t pos_padre) {
	return pos_padre * 2 + 2;
}

size_t pos_hijo_izq(size_t pos_padre) {
	return pos_padre * 2 + 1;
}

// Hace upheap al dato en la posición n del arreglo.
void upheap(void** datos, size_t n, cmp_func_t cmp) {
	return;
}

// Hace downheap al dato en la posición n del arreglo.
void downheap(void** datos, size_t n, size_t tam, cmp_func_t cmp) {
	return;
}

/* ******************************************************************
 *                  		PRIMITIVAS HEAP
 * *****************************************************************/

heap_t *heap_crear(cmp_func_t cmp) {
	heap_t* tmp = NULL;
	return tmp;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
	heap_t* tmp = NULL;
	return tmp;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)) {
	return;
}

size_t heap_cantidad(const heap_t *heap) {
	return 0;
}

bool heap_esta_vacio(const heap_t *heap) {	
	return false;
}

bool heap_encolar(heap_t *heap, void *elem) {
	return false;
}

void *heap_ver_max(const heap_t *heap) {
	return NULL;
}

void *heap_desencolar(heap_t *heap) {
	return NULL;
}

/* ******************************************************************
 *                  			HEAPSORT
 * *****************************************************************/

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {
	return;
}