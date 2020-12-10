#include "testing.h"
#include "heap.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int cmp_int(const void* int_1, const void* int_2) {
	return *((int*) int_1) - *((int*) int_2);
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
	int arr_int[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int* arr[] = {arr_int, arr_int + 1, arr_int + 2, arr_int + 3, arr_int + 4, arr_int + 5, 
		arr_int + 6, arr_int + 7, arr_int + 8, arr_int + 9};
	for(size_t i=0; i<8; i++) {
		heap_encolar(heap,arr[i]);
	}
	print_test("cantidad de elementos correcta", heap_cantidad(heap) == 8);
	print_test("ver maximo correcto", *(int*) heap_ver_max(heap) == 7);
	
	print_test("encolar nuevo maximo correcto", heap_encolar(heap, arr[8]) && *(int*) heap_ver_max(heap) == 8);
	print_test("encolar correcto", heap_encolar(heap,arr[9]) && *(int*) heap_ver_max(heap) == 9);
	print_test("cantidad de elementos correcta", heap_cantidad(heap) == 10);
	heap_destruir(heap,NULL);
}

void prueba_heap_desencolar() {
	heap_t* heap = heap_crear(cmp_int);
	if(!heap) return;
	int arr_int[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	int* arr[] = {arr_int, arr_int + 1, arr_int + 2, arr_int + 3, arr_int + 4, arr_int + 5, 
		arr_int + 6, arr_int + 7, arr_int + 8, arr_int + 9};
	for(size_t i=0; i<9; i++) {
		heap_encolar(heap, arr[i]);
	}
	print_test("desencolar correcto", *(int*) heap_desencolar(heap) == 8);
	print_test("cantidad de elementos correcta", heap_cantidad(heap) == 8);
	print_test("desencolar correcto", *(int*) heap_desencolar(heap) == 7);
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

void pruebas_heap_sort() {
	int arr_int[] = {0, 23, -4, 142512};
	int* arr[] = {arr_int, arr_int + 1, arr_int + 2, arr_int + 3};

	heap_sort((void**) arr, 1, cmp_int);
	print_test("Heap sort un elemento", **arr == 0);

	heap_sort((void**) arr, 4, cmp_int);
	print_test("Heap sort varios elementos", *arr[0] == -4 && *arr[1] == 0 && *arr[2] == 23 && *arr[3] == 142512);
}

void pruebas_heap_crear_arr() {
	int arr_int[] = {0, 23, -4, 142512};
	int* arr[] = {arr_int, arr_int + 1, arr_int + 2, arr_int + 3};

	heap_t* heap = heap_crear_arr((void**) arr, 1, cmp_int);
	print_test("Crear heap con arreglo, no esta vacio", !heap_esta_vacio(heap));
	print_test("Crear heap con arreglo de 1 elemento, cantidad es 1", heap_cantidad(heap) == 1);
	print_test("Crear heap con arreglo de 1 elemento, maximo es ese elemento", *(int*)heap_ver_max(heap) == 0);
	heap_destruir(heap, NULL);


	heap = heap_crear_arr((void**) arr, 4, cmp_int);
	print_test("Crear heap con arreglo de varios elemento, cantidad correcta", heap_cantidad(heap) == 4);
	print_test("Crear heap con arreglo de varios elementos, desencola en orden correcto", *(int*)heap_desencolar(heap) == 142512 &&
		*(int*)heap_desencolar(heap) == 23 && *(int*)heap_desencolar(heap) == 0 && *(int*)heap_desencolar(heap) == -4);
	print_test("Crear heap con arreglo de varios elementos, no modificó arreglo original", arr[0] == arr_int &&
		arr[1] == arr_int + 1 && arr[2] == arr_int + 2 && arr[3] == arr_int + 3);
	heap_destruir(heap, NULL);
}

void pruebas_heap_vol_encolar(size_t largo) {
	heap_t* heap = heap_crear(cmp_int);

	bool ok = true;
	for (size_t i = 0; i < largo; i++) {
		int* dato = malloc(sizeof(int));
		*dato = rand();
		ok = heap_encolar(heap, dato);
	}

	for (size_t i = 0; i < largo - 1; i++) {
		int* actual = heap_desencolar(heap);
		ok = cmp_int(actual, heap_ver_max(heap)) >= 0;
		free(actual);
	}

	print_test("Prueba volumen encolar y desencolar en orden correcto", ok);
	heap_destruir(heap, free);
}

void pruebas_heap_vol_destruir(size_t largo) {
	heap_t* heap = heap_crear(cmp_int);

	for (size_t i = 0; i < largo; i++) {
		int* dato = malloc(sizeof(int));
		heap_encolar(heap, dato);
	}

	heap_destruir(heap, free);
	print_test("Prueba volumen destruir, libera todos los datos (ver valgrind)", true);
}


void pruebas_heap_sort_vol(size_t largo) {
	int** arr = malloc(sizeof(int*) * largo);
	for (size_t i = 0; i < largo; i++) {
		arr[i] = malloc(sizeof(int));
		*arr[i] = rand();
	}

	heap_sort((void**) arr, largo, cmp_int);
	bool ok = true;
	for (size_t i = 0; i < largo - 1; i++) {
		ok = cmp_int(arr + i, arr + i + 1) <= 0;
		free(arr[i]);
	}

	print_test("Prueba volumen heap sort, ordena correctamente", ok);
	free(arr[largo - 1]);
	free(arr);
}

void pruebas_heap_vol_arr(size_t largo) {
	int** arr = malloc(sizeof(int*) * largo);
	for (size_t i = 0; i < largo; i++) {
		arr[i] = malloc(sizeof(int));
		*arr[i] = rand();
	}

	heap_t* heap = heap_crear_arr((void**) arr, largo, cmp_int);
	free(arr);

	bool ok = true;
	for (size_t i = 0; i < largo - 1; i++) {
		int* actual = heap_desencolar(heap);
		ok = cmp_int(actual, heap_ver_max(heap)) >= 0;
		free(actual);
	}

	print_test("Prueba volumen heap_crear_arr(), denencolar en orden correcto", ok);
	heap_destruir(heap, free);
}

void pruebas_heap_estudiante(void) {
	srand((unsigned) time(NULL));
  printf("Pruebas heap vacio\n");
  prueba_heap_vacio();
  printf("\nPruebas heap encolar, desencolar y destruccion()\n");
	prueba_heap_encolar();
	prueba_heap_desencolar();
	prueba_heap_destruir();
	printf("\nPruebas heap_sort()\n");
	pruebas_heap_sort();
	printf("\nPruebas heap_crear_arr()\n");
	pruebas_heap_crear_arr();

	printf("\nPruebas de volumen\n");
	pruebas_heap_vol_encolar(10000);
	pruebas_heap_sort_vol(10000);
	pruebas_heap_vol_destruir(10000);
	pruebas_heap_vol_arr(10000);
	printf("\n");
}

