#include "testing.h"
#include "heap.h"
#include <stdlib.h>
#include <stdio.h>

int cmp_int(const void* int_1, const void* int_2){
	const int* a = int_1;
	const int* b = int_2;
	return (*a - *b);
}	
void prueba_heap_vacio() {
	heap_t* heap = heap_crear(cmp_int);
	if(!heap) return;
	print_test("se creo heap y esta vacio",heap_esta_vacio(heap) && (heap_cantidad(heap) == 0));
	print_test("desencolar devuelve NULL",!heap_desencolar(heap));
	heap_destruir(heap,NULL);
}
void prueba_heap_encolar() {
	heap_t* heap = heap_crear(cmp_int);
	if(!heap) return;
	for(size_t i=0; i<9; i++) {
		heap_encolar(heap,(void*)i);
	}
	print_test("cantidad de elementos correcta", heap_cantidad(heap) == 9);
	print_test("ver maximo correcto", heap_ver_max(heap) == (void*)8);
	
	print_test("encolar nuevo maximo correcto", heap_encolar(heap,(void*) 10) && heap_ver_max(heap) == (void*)10);
	print_test("encolar correcto", heap_encolar(heap,(void*) 9) && heap_ver_max(heap) == (void*)9);
	print_test("cantidad de elementos correcta", heap_cantidad(heap) == 11);
	heap_destruir(heap,NULL);
}

void prueba_heap_desencolar() {
	heap_t* heap = heap_crear(cmp_int);
	if(!heap) return;
	for(size_t i=0; i<9; i++) {
		heap_encolar(heap,(void*)i);
	}
	print_test("desencolar correcto", heap_desencolar(heap) == (void*)8);
	print_test("cantidad de elementos correcta", heap_cantidad(heap) == 8);
	print_test("desencolar correcto", heap_desencolar(heap) == (void*)7);
	print_test("cantidad de elementos correcta", heap_cantidad(heap) == 7);
	print_test("no esta vacio", !heap_esta_vacio(heap));
	while(!heap_esta_vacio(heap)) {
		heap_desencolar(heap);
	}
	print_test("se vació el heap", heap_esta_vacio(heap));
	heap_destruir(heap,NULL);
}

void prueba_heap_destruir() {
	heap_t* heap = heap_crear(cmp_int);
	if(!heap) return;
	int* a = malloc(sizeof(int));
	int* b = malloc(sizeof(int));
	int* c = malloc(sizeof(int));
	int* d = malloc(sizeof(int));
	heap_encolar(heap,(void*)a);
	heap_encolar(heap,(void*)b);
	heap_encolar(heap,(void*)c);
	heap_encolar(heap,(void*)d);
	heap_destruir(heap,free);
}

void pruebas_heap_estudiante(void){
	prueba_heap_vacio();
	prueba_heap_encolar();
	prueba_heap_desencolar();
	prueba_heap_destruir();
	return;
}
