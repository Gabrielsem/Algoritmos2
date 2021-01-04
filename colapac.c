#include "colapac.h"
#include "heap.h"
#include "cola.h"
#include "hash.h"

struct colapac {
	hash_t* especialidad;
};

typedef struct colaesp {
	cola_t* urgencias;
	heap_t* regulares;
} colaesp_t;

colapac_t* colapac_crear(void) {
	colapac_t* colapac = NULL;

	return colapac;
}

void colapac_destruir(colapac_t* colapac) {
	return;
}

void colapac_encolar(colapac_t* colapac, char* nombre, const char* especialidad, bool urgente) {
	return;
}

size_t colapac_cantidad(colapac_t* colapac) {
	return 0;
}

char* colapac_desencolar(colapac_t* colapac, const char* especialidad) {
	return NULL;
}