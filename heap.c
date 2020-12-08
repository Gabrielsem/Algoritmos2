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
	return (pos_hijo - 1)/2;
}

size_t pos_hijo_der(size_t pos_padre) {
	return pos_padre * 2 + 2;
}

size_t pos_hijo_izq(size_t pos_padre) {
	return pos_padre * 2 + 1;
}

void swap(void** dato_1, void** dato_2){
	void* aux = *dato_1;
	*dato_1 = *dato_2;
	*dato_2 = aux;
}

// Hace upheap al dato en la posición n del arreglo datos.
void upheap(void** datos, size_t n, cmp_func_t cmp) {
	if (n == 0)
		return;

	size_t padre = pos_padre(n);
	if(cmp(datos[n], datos[padre]) > 0){
		swap(datos + n, datos + padre);
		upheap(datos, padre, cmp);
	}
}

// Hace downheap al primer dato del arreglo datos de tamaño n.
void downheap(void** datos, size_t n, cmp_func_t cmp) {
	size_t izq = pos_hijo_izq(0);
	size_t der = pos_hijo_der(0);

	if(izq < n && cmp(datos[izq], *datos) > 0){
		swap(datos, datos + izq);
		downheap(datos + izq, n - izq, cmp);
	} else if (der < n && cmp(datos[der], *datos) > 0){
		swap(datos, datos + der);
		downheap(datos + der, n - der, cmp);
	}
}

void** dup_arreglo(void** original, size_t n){
	void** copia = malloc(sizeof(void*) * n);
	if (!copia)
		return NULL;

	for (size_t i = 0; i < n; i++){
		copia[i] = original[i];
	}

	return copia;
}

void heapify(void** datos, size_t n, cmp_func_t cmp){
	for (size_t i = n / 2; i <= n; i++){
		downheap(datos + n - i, n, cmp);
	}
}

/* ******************************************************************
 *                  		PRIMITIVAS HEAP
 * *****************************************************************/

heap_t *heap_crear(cmp_func_t cmp) {
	heap_t* tmp = NULL;
	return tmp;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
	heap_t* heap = malloc(sizeof(heap_t));
	if (!heap)
		return NULL;

	heap->datos = dup_arreglo(arreglo, n);
	if (!heap->datos){
		free(heap);
		return NULL;
	}

	heapify(heap->datos, n, cmp);

	heap->cmp = cmp;
	heap->cant = heap->cap = n;
	return heap;
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
	heapify(elementos, cant, cmp);

	for(size_t i = 0; i < cant - 1; i++){
		swap(elementos, elementos + cant - 1 - i);
		downheap(elementos, cant - i, cmp);
	}
}