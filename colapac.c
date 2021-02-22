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

typedef struct paciente {
	char* nombre;
	unsigned short anio_ant;
} paciente_t;

/*
  -------------- FUNCIONES INTERNAS  -------------- 
*/

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

// Destruye una cola de una especialidad y todos sus pacientes, si es que la cola existe
void destruir_colaesp(void* colaesp) {
	if (!colaesp) return;

	cola_destruir(((colaesp_t*) colaesp)->urgentes, free);
	heap_destruir(((colaesp_t*) colaesp)->regulares, destruir_paciente);
	free(colaesp);
}

// Crea un paciente con el nombre y año de antiguedad dados
// Devuelve NULL si falla
paciente_t* crear_paciente(char* nombre, unsigned short anio_ant) {
	paciente_t* paciente = malloc(sizeof(paciente_t));
	if (!paciente) return NULL;

	paciente->nombre = nombre;
	paciente->anio_ant = anio_ant;
	return paciente;
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

// Encola un paciente con la especialidad y urgencia dada.
// Devuelve false si falla (paciente o especialidad no existe o fallo de memoria),
// o true si se logra encolar.
// Si se destruye la clínica, se hace free() al nombre del paciente.
bool colapac_encolar(colapac_t* colapac, char* nombre, const char* especialidad, const unsigned short antig, bool urgente) {
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
		encolo = cola_encolar(colaesp->urgentes, nombre);
	} else {
		paciente_t* paciente = crear_paciente(nombre, antig);
		if (!paciente) return false;
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

char* colapac_desencolar(colapac_t* colapac, const char* especialidad) {
	colaesp_t* colaesp = hash_obtener(colapac->colas, especialidad);
	if (!colaesp) return NULL;

	char* nombre = cola_desencolar(colaesp->urgentes);
	if(!nombre) {
		paciente_t* paciente = heap_desencolar(colaesp->regulares);
		if (!paciente) return NULL;

		nombre = paciente->nombre;
		free(paciente);
	}

	colaesp->cant_en_espera--;
	return nombre;
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