#include "funciones_tp2.h"
#include "mensajes.h"
#include "csv.h"
#include <string.h>
#include <stdlib.h>
#define _POSIX_C_SOURCE 200809L

#define URGENCIA_URGENTE "URGENTE"
#define URGENCIA_REGULAR "REGULAR"

void pedir_turno(const char** parametros, colapac_t* colapac, hash_t* especialidades) {
	bool urgente = strcmp(parametros[1], URGENCIA_URGENTE) == 0;
	bool regular = strcmp(parametros[2], URGENCIA_REGULAR) == 0;
	if (!urgente && !regular) {
		printf(ENOENT_URGENCIA, parametros[2]);
		return;
	}

	if(!colapac_existe(colapac, parametros[0])) {
		printf(ENOENT_PACIENTE, parametros[0]);
		return;
	}

	if (!hash_pertenece(especialidades, parametros[1])) {
		printf(ENOENT_ESPECIALIDAD, parametros[1]);
		return;
	}

	char* nombre = strdup(parametros[0]);
	if (!nombre || !colapac_encolar(colapac, nombre, parametros[1], urgente)) {
		printf(ERR_MEM);
		return;
	}
 
	printf(PACIENTE_ENCOLADO, parametros[0]);
	printf(CANT_PACIENTES_ENCOLADOS, colapac_cantidad(colapac, parametros[1]), parametros[1]);
}

void atender_siguiente(const char** parametros, colapac_t* colapac, abb_t* doctores) {
	datos_doctor_t* doctor = abb_obtener(doctores, parametros[0]);
	if (!doctor) {
		printf(ENOENT_DOCTOR, parametros[0]);
		return;
	}

	char* nombre = colapac_desencolar(colapac, doctor->especialidad);
	if (!nombre) {
		printf(SIN_PACIENTES);
		return;
	}

	printf(PACIENTE_ATENDIDO, nombre);
	printf(CANT_PACIENTES_ENCOLADOS, colapac_cantidad(colapac, doctor->especialidad), doctor->especialidad);
	free(nombre);
	doctor->pacientes_atendidos++;
}

void contar_abb(const char* clave, void* dato, void* cantidad) {
	*((size_t*) cantidad) += 1;
}

void imprimir_doctores(const char* clave, void* dato, void* indice) {
	datos_doctor_t* doctor = dato;
	printf(INFORME_DOCTOR, *((size_t*) indice), clave, doctor->especialidad, doctor->pacientes_atendidos);
	*((size_t*) indice) += 1;
}

void informe_doctores(const char** parametros, abb_t* doctores) {
	const char* ini = parametros[0][0] == '\0' ? NULL : parametros[0];
	const char* fin = parametros[1][0] == '\0' ? NULL : parametros[1];

	size_t cantidad_doctores = 0;
	abb_in_order_rng(doctores, contar_abb, &cantidad_doctores, ini, fin);

	size_t indice = 1;
	printf(DOCTORES_SISTEMA, cantidad_doctores);
	abb_in_order_rng(doctores, imprimir_doctores, &indice, ini, fin);
}