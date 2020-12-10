#include "testing.h"
#include "heap.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int cmp_int(const void* int_1, const void* int_2) {
	return *((int*) int_1) - *((int*) int_2);
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