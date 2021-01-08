#include "colapac.h"
#include "dependencias/heap.h"
#include "dependencias/cola.h"
#include "dependencias/hash.h"
#include <stdlib.h>
#include <string.h>
#define _POSIX_C_SOURCE 200809L

/*
  -------------- DEFINICION DE LAS ESTRUCTURAS  -------------- 
*/

typedef struct paciente {
	char* nombre;
	unsigned short anio_ant;
} paciente_t;

// cola general (diccionario con colas para cada especialidad)
struct colapac {
	hash_t* esp;
	hash_t* antig;
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

// Crea un paciente con el nombre y año de antiguedad dados
// Devuelve NULL si falla
paciente_t* crear_paciente(char* nombre, unsigned short anio_ant) {
	paciente_t* paciente = malloc(sizeof(paciente_t));
	if (!paciente) return NULL;

	paciente->nombre = nombre;
	paciente->anio_ant = anio_ant;
	return paciente;
}

// Compara la antiguedad de dos pacientes. Devuelve:
// 0 si es la misma antiguedad
// > 0 si el paciente 1 tiene mas antiguedad que el paciente 2
// < 0 si el paciente 1 tiene menos antiguedad que el paciente 2
int antiguedad_cmp(const void* paciente_1, const void* paciente_2) {
	int anio_1 = ((paciente_t*) paciente_1)->anio_ant;
	int anio_2 = ((paciente_t*) paciente_2)->anio_ant;
	return anio_2 - anio_1;
}

// Destruye un paciente, liberando también el nombre almacenado
void destruir_paciente(void* paciente) {
	free(((paciente_t*) paciente)->nombre);
	free(paciente);
}

// Destruye una cola de una especialidad y todos sus pacientes
void destruir_colaesp(void* colaesp) {
	cola_destruir(((colaesp_t*) colaesp)->urgentes, free);
	heap_destruir(((colaesp_t*) colaesp)->regulares, destruir_paciente);
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

	colaesp->regulares = heap_crear(antiguedad_cmp);
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

colapac_t* colapac_crear(hash_t* antiguedades) {
	colapac_t* colapac = malloc(sizeof(colapac_t));
	if (!colapac) return NULL;

	colapac->esp = hash_crear(destruir_colaesp);
	if (!colapac->esp) {
		free(colapac);
		return NULL;
	}

	colapac->antig = antiguedades;
	return colapac;
}

bool colapac_existe(colapac_t* colapac, const char* paciente) {
	return hash_pertenece(colapac->antig, paciente);
}

bool colapac_encolar(colapac_t* colapac, char* nombre, const char* especialidad, bool urgente) {
	unsigned short* anio = hash_obtener(colapac->antig, nombre);
	if (!anio) return false;

	colaesp_t* colaesp = hash_obtener(colapac->esp, especialidad);
	if (!colaesp) {
		colaesp = crear_colaesp();
		if (!colaesp || !hash_guardar(colapac->esp, especialidad, colaesp)) return false;
	}

	bool encolo = false;
	if (urgente) {
		encolo = cola_encolar(colaesp->urgentes, nombre);
	} else {
		paciente_t* paciente = crear_paciente(nombre, *anio);
		if (!paciente) return false;
		encolo = heap_encolar(colaesp->regulares, paciente);
	}

	if (encolo) colaesp->en_espera++;
	return encolo;
}

void colapac_destruir(colapac_t* colapac) {
	hash_destruir(colapac->esp);
	free(colapac);
}

size_t colapac_cantidad(colapac_t* colapac, const char* especialidad) {
	colaesp_t* colaesp = hash_obtener(colapac->esp, especialidad);
	if (!colaesp) {
		return 0;
	}
	return colaesp->en_espera;
}

char* colapac_desencolar(colapac_t* colapac, const char* especialidad) {
	colaesp_t* colaesp = hash_obtener(colapac->esp, especialidad);
	if (!colaesp)
		return NULL;

	char* nombre = cola_desencolar(colaesp->urgentes);
	if(!nombre) {
		paciente_t* paciente = heap_desencolar(colaesp->regulares);
		if (!paciente) return NULL;
		nombre = paciente->nombre;
		free(paciente);
	}

	colaesp->en_espera--;
	return nombre;
}
