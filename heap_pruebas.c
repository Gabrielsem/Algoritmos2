#include "testing.h"
#include "heap.h"
#include <stdio.h>

int cmp_int(const void* int_1, const void* int_2){
	return *((int*) int_1) - *((int*) int_2);
}

void pruebas_heap_sort(){
	int arr[10] = {4, 23, -4, 2, 54, 1425525324, 0, -4353, 17, -1};
	heap_sort((void**) arr, 10, cmp_int);
	for(size_t i = 0; i < 10; i++){
		printf("%i ", arr[i]);
	}
}

void pruebas_heap_estudiante(void){
	printf("\nPruebas heap_sort()\n");
	pruebas_heap_sort();
	return;
}