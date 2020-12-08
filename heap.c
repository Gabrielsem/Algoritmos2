#include "heap.h"
#include <stdlib.h>


/* ******************************************************************
 *                   DEFINICIÓN DE LAS ESTRUCTURAS
 * *****************************************************************/

#define CAPACIDAD_INICIAL 10
#define PORC_AGRANDAR 70
#define PORC_ACHICAR 15
#define FACTOR_REDIM 2

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
	return (pos_hijo - 1)/2;
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

size_t nueva_cap(const heap_t* heap){
	size_t cap = heap->cap;
	size_t porcentaje =  heap->cant*100/heap->cap;

	if(porcentaje < PORC_ACHICAR){
		cap = heap->cap/FACTOR_REDIM;
		if(cap < CAPACIDAD_INICIAL)
			cap = CAPACIDAD_INICIAL;
	}

	if(porcentaje > PORC_AGRANDAR){
		cap = heap->cap * FACTOR_REDIM;
	}

	return cap;
}

bool heap_redim(heap_t *heap){
	size_t cap = nueva_cap(heap);
	if(cap == heap->cap) return true;

	void** datos_nuevo = realloc(heap->datos,cap * sizeof(void*));
	if(datos_nuevo == NULL) return false;
	heap->datos = datos_nuevo;
	heap->cap = cap;
	return true;
}

/* ******************************************************************
 *                  		PRIMITIVAS HEAP
 * *****************************************************************/

heap_t *heap_crear(cmp_func_t cmp) {
	heap_t* heap = malloc(sizeof(heap_t));
	if(!heap) return NULL;
	heap->cap = CAPACIDAD_INICIAL;
	heap->cant = 0;
	heap->datos = malloc(sizeof(void*) * heap->cap);
	if(!heap->datos) {
		free(heap);
		return NULL;
	}
	return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
	heap_t* tmp = NULL;
	return tmp;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)) {
	if(destruir_elemento) {
		for(size_t i = 0; i < heap->cant; i++) {
			destruir_elemento(heap->datos[i]);
		}
	}
	free(heap->datos);
	free(heap);
}

size_t heap_cantidad(const heap_t *heap) {
	return heap->cant;
}

bool heap_esta_vacio(const heap_t *heap) {	
	return heap->cant ? false : true;
}

bool heap_encolar(heap_t *heap, void *elem) {
	if(!heap_redim(heap)) return false;
	heap->datos[heap->cant] = elem;
	upheap(heap->datos, heap->cant, heap->cmp);
	heap->cant += 1;
	return true;
}

void *heap_ver_max(const heap_t *heap) {
	return heap->datos[0];
}

void *heap_desencolar(heap_t *heap) {
	if(!heap_redim(heap)) return NULL;
	void* dato = heap->datos[0];
	heap->cant -= 1;
	heap->datos[0] = heap->datos[heap->cant];
	downheap(heap->datos, (size_t)0, heap->cant, heap->cmp);
	return dato;
}

/* ******************************************************************
 *                  			HEAPSORT
 * *****************************************************************/

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {
	return;
}