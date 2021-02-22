#include "doctor.h"
#include <stdlib.h>
#include <string.h>
#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE 1
#define _XOPEN_SOURCE 500

struct doctor {
	char* nombre;
	char* especialidad;
	size_t pacientes_atendidos;
};

doctor_t* doctor_crear(const char* nombre, const char* especialidad) {
	doctor_t* doctor = malloc(sizeof(doctor_t));
	if (!doctor) return NULL;

	doctor->nombre = strdup(nombre);
	if (!nombre) {
		free(doctor);
		return NULL;
	}

	doctor->especialidad = strdup(especialidad);
	if (!especialidad) {
		free(doctor->nombre);
		free(doctor);
		return NULL;
	}

	doctor->pacientes_atendidos = 0;
	return doctor;
}

// Destruye los datos de un doctor
void doctor_destruir(doctor_t* doctor) {
	free(doctor->especialidad);
	free(doctor->nombre);
	free(doctor);
}

size_t doctor_cant_atendidos(doctor_t* doctor) {
	return doctor->pacientes_atendidos;
}

const char* doctor_especialidad(doctor_t* doctor) {
	return doctor->especialidad;
}

const char* doctor_nombre(doctor_t* doctor) {
	return doctor->nombre;
}

void doctor_contar_paciente(doctor_t* doctor) {
	doctor->pacientes_atendidos++;
}