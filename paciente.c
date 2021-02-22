#include "paciente.h"
#include <stdlib.h>
#include "string.h"
#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE 1
#define _XOPEN_SOURCE 500

struct paciente {
	char* nombre;
	unsigned short anio_ant;
};


// Crea un paciente con el nombre y año de antiguedad dados
// Devuelve NULL si falla
paciente_t* paciente_crear(const char* nombre, unsigned short anio_ant) {
	paciente_t* paciente = malloc(sizeof(paciente_t));
	if (!paciente) return NULL;

	paciente->nombre = strdup(nombre);
	if (!paciente->nombre) {
		free(paciente);
		return NULL;
	}

	paciente->anio_ant = anio_ant;
	return paciente;
}

// Destruye un paciente.
void paciente_destruir(paciente_t* paciente) {
	free(paciente->nombre);
	free(paciente);
}

int paciente_antiguedad_cmp(const paciente_t* paciente_1, const paciente_t* paciente_2) {
	int anio_1 = paciente_1->anio_ant;
	int anio_2 = paciente_2->anio_ant;
	return anio_2 - anio_1;
}

const char* paciente_obtener_nombre(paciente_t* paciente) {
	return paciente->nombre;
}