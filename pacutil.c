#include "pacutil.h"
#include "heap.h"
#include "cola.h"
#include "hash.h"
#include <stdlib.h>
#include <string.h>
#define _POSIX_C_SOURCE 200809L

/* ******************************************************************
 *                            TDA PACIENTE
 * *****************************************************************/

/*
  -------------- DEFINICION DE LAS ESTRUCTURAS  -------------- 
*/

struct paciente {
	char* nombre;
	unsigned short anio_ant;
};

/*
  -------------- PRIMITIVAS TDA  -------------- 
*/

paciente_t* crear_paciente(const char* nombre, unsigned short anio_ant) {
	paciente_t* paciente = malloc(sizeof(paciente));
	if (!paciente) return NULL;

	paciente->nombre = strdup(nombre);
	if (!paciente->nombre) {
		free(paciente);
		return NULL;
	}

	paciente->anio_ant = anio_ant;
	return paciente;
}

const char* nombre_paciente(paciente_t* paciente) {
	return paciente->nombre;
}

void destruir_paciente(void* paciente) {
	paciente_t* pac = paciente;
	free(pac->nombre);
	free(pac);
}

int antiguedad_cmp(const paciente_t* paciente_1, const paciente_t* paciente_2) {
	int anio_1 = paciente_1->anio_ant;
	int anio_2 = paciente_2->anio_ant;
	return anio_2 - anio_1;
}

/* ******************************************************************
 *                       TDA COLA DE PACIENTES
 * *****************************************************************/

/*
  -------------- DEFINICION DE LAS ESTRUCTURAS  -------------- 
*/

// cola general (diccionario con colas para cada especialidad)
struct colapac {
	hash_t* esp;
};

// cola de una especialidad
typedef struct colaesp {
	cola_t* urgentes;
	heap_t* regulares;
	size_t en_espera;
} colaesp_t;


/*
  -------------- FUNCIONES INTERNAS  -------------- 
*/

// Wrapper de antiguedad_cmp() con void* para utilizarla en el TDA Heap
int antig_cmp_wr(const void* paciente_1, const void* paciente_2) {
	return antiguedad_cmp((const paciente_t*) paciente_1, (const paciente_t*) paciente_2);
}

// Destruye una cola de una especialidad y todos sus pacientes
void destruir_colaesp(colaesp_t* colaesp) {
	cola_destruir(colaesp->urgentes, destruir_paciente);
	heap_destruir(colaesp->regulares, destruir_paciente);
	free(colaesp);
}

// Wrapper de destruir_colaesp() para utilizarla en el TDA Hash
void destr_colaesp_wr(void* colaesp) {
	destruir_colaesp((colaesp_t*) colaesp);
}

/* Crea una cola para una especialidad y la devuelve. Si falla la creación,
 * devuelve NULL
 */
colaesp_t* crear_colaesp() {
	colaesp_t* colaesp = malloc(sizeof(colaesp_t));
	if (!colaesp) return NULL;

	colaesp->urgentes = cola_crear();
	if (!colaesp->urgentes) {
		free(colaesp);
		return NULL;
	}

	colaesp->regulares = heap_crear(antig_cmp_wr);
	if (!colaesp->regulares) {
		cola_destruir(colaesp->urgentes, NULL);
		free(colaesp);
		return NULL;
	}

	colaesp->en_espera = 0;
	return colaesp;
}

/*
  -------------- PRIMITIVAS TDA  -------------- 
*/

colapac_t* colapac_crear(void) {
	colapac_t* colapac = malloc(sizeof(colapac_t));
	if (!colapac) return NULL;

	colapac->esp = hash_crear(destr_colaesp_wr);
	if (!colapac->esp) {
		free(colapac);
		return NULL;
	}

	return colapac;
}

bool colapac_encolar(colapac_t* colapac, paciente_t* paciente, const char* especialidad, bool urgente) {
	colaesp_t* colaesp = NULL;
	if (!hash_pertenece(colapac->esp, especialidad)) {
		colaesp = crear_colaesp();
		if (!colaesp) return false;
	} else {
		colaesp = hash_obtener(colapac->esp, especialidad);
	}

	bool encolo = false;
	encolo = (urgente && cola_encolar(colaesp->urgentes, paciente)) ||
		(!urgente && heap_encolar(colaesp->regulares, paciente));

	if (encolo) colaesp->en_espera++;
	return encolo;
}

void colapac_destruir(colapac_t* colapac) {
	hash_destruir(colapac->esp);
	free(colapac);
}

size_t colapac_cantidad(colapac_t* colapac, const char* especialidad) {
	colaesp_t* colaesp = hash_obtener(colapac->esp, especialidad);
	if (!colaesp) 
		return 0;
	return colaesp->en_espera;
}

paciente_t* colapac_desencolar(colapac_t* colapac, const char* especialidad) {
	colaesp_t* colaesp = hash_obtener(colapac->esp, especialidad);
	if (!colaesp)
		return NULL;

	paciente_t* paciente = NULL;
	if((paciente = cola_desencolar(colaesp->urgentes)) || (paciente = heap_desencolar(colaesp->regulares))){
		colaesp->en_espera--;
	}

	return paciente;
}