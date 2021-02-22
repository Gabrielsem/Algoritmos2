#include "colapac.h"
#include <stdlib.h>
#include "dependencias/heap.h"
#include "dependencias/cola.h"
#include "dependencias/hash.h"

struct colapac {
	hash_t* colas; // Diccionario de colaesp_t*. Clave: especialidad.
};

// cola de una especialidad
typedef struct colaesp {
	cola_t* urgentes;
	heap_t* regulares;
	size_t cant_en_espera;
} colaesp_t;

/*
  -------------- FUNCIONES INTERNAS  -------------- 
*/

// Wrapper de paciente_antiguedad_cmp() del TDA Paciente para usar en el Heap.
int paciente_cmp_wrp(const void* paciente_1, const void* paciente_2) {
	return paciente_antiguedad_cmp(paciente_1, paciente_2);
}

// Destruye una cola de una especialidad.
void destruir_colaesp(void* colaesp) {
	if (!colaesp) return;

	cola_destruir(((colaesp_t*) colaesp)->urgentes, NULL);
	heap_destruir(((colaesp_t*) colaesp)->regulares, NULL);
	free(colaesp);
}

// Crea una cola para una especialidad y la devuelve. Si falla la creación,
// devuelve NULL
colaesp_t* crear_colaesp() {
	colaesp_t* colaesp = malloc(sizeof(colaesp_t));
	if (!colaesp) return NULL;

	colaesp->urgentes = cola_crear();
	if (!colaesp->urgentes) {
		free(colaesp);
		return NULL;
	}

	colaesp->regulares = heap_crear(paciente_cmp_wrp);
	if (!colaesp->regulares) {
		cola_destruir(colaesp->urgentes, NULL);
		free(colaesp);
		return NULL;
	}

	colaesp->cant_en_espera = 0;
	return colaesp;
}

/*
  -------------- PRIMITIVAS TDA  -------------- 
*/

colapac_t* colapac_crear() {
	colapac_t* colapac = malloc(sizeof(colapac_t));
	if (!colapac) return NULL;

	colapac->colas = hash_crear(destruir_colaesp);
	if (!colapac->colas) {
		free(colapac);
		return NULL;
	}

	return colapac;
}

bool colapac_encolar(colapac_t* colapac, paciente_t* paciente, const char* especialidad, bool urgente) {
	if (!hash_pertenece(colapac->colas, especialidad)) return false;

	colaesp_t* colaesp = hash_obtener(colapac->colas, especialidad);
	if (!colaesp) {
		colaesp = crear_colaesp();
		if (!colaesp) return false;

		if (!hash_guardar(colapac->colas, especialidad, colaesp)) {
			destruir_colaesp(colaesp);
			return false;
		}
	}

	bool encolo = false;
	if (urgente) {
		encolo = cola_encolar(colaesp->urgentes, paciente);
	} else {
		encolo = heap_encolar(colaesp->regulares, paciente);
	}

	if (encolo) colaesp->cant_en_espera++;
	return encolo;
}

bool colapac_agregar(colapac_t* colapac, const char* especialidad) {
	return hash_pertenece(colapac->colas, especialidad) || hash_guardar(colapac->colas, especialidad, NULL);
}

bool colapac_existe(colapac_t* colapac, const char* especialidad) {
	return hash_pertenece(colapac->colas, especialidad);
}

paciente_t* colapac_desencolar(colapac_t* colapac, const char* especialidad) {
	colaesp_t* colaesp = hash_obtener(colapac->colas, especialidad);
	if (!colaesp) return NULL;

	paciente_t* paciente = cola_desencolar(colaesp->urgentes);
	if(!paciente) {
		paciente = heap_desencolar(colaesp->regulares);
		if (!paciente) return NULL;
	}

	colaesp->cant_en_espera--;
	return paciente;
}

void colapac_destruir(colapac_t* colapac) {
	hash_destruir(colapac->colas);
	free(colapac);
}

size_t colapac_cantidad(colapac_t* colapac, const char* especialidad) {
	colaesp_t* colaesp = hash_obtener(colapac->colas, especialidad);
	if (!colaesp) {
		return 0;
	}

	return colaesp->cant_en_espera;
}