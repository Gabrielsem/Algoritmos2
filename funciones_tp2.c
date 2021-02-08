#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE 1
#define _XOPEN_SOURCE 500
#include "funciones_tp2.h"
#include "mensajes.h"
#include "clinica.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define URGENCIA_URGENTE "URGENTE"
#define URGENCIA_REGULAR "REGULAR"

void pedir_turno(const char** parametros, clinica_t* clinica) {
	bool urgente = strcmp(parametros[2], URGENCIA_URGENTE) == 0;
	bool regular = strcmp(parametros[2], URGENCIA_REGULAR) == 0;
	if (!urgente && !regular) {
		printf(ENOENT_URGENCIA, parametros[2]);
		return;
	}

	char* nombre = strdup(parametros[0]);
	if (!nombre) {
		printf(ERR_MEM);
		return;
	}

	if(clinica_encolar(clinica, nombre, parametros[1], urgente)) {
		printf(PACIENTE_ENCOLADO, parametros[0]);
		printf(CANT_PACIENTES_ENCOLADOS, clinica_cantidad_pac(clinica, parametros[1]), parametros[1]);
		return;
	}

	if(!clinica_existe_pac(clinica, parametros[0])) {
		printf(ENOENT_PACIENTE, parametros[0]);
	} else if (!clinica_existe_esp(clinica, parametros[1])) {
		printf(ENOENT_ESPECIALIDAD, parametros[1]);
	}
	free(nombre);
}

void atender_siguiente(const char** parametros, clinica_t* clinica) {
	char* nombre = clinica_desencolar(clinica, parametros[0]);
	if (nombre) {
		printf(PACIENTE_ATENDIDO, nombre);
		const char* especialidad = clinica_especialidad(clinica, parametros[0]);
		printf(CANT_PACIENTES_ENCOLADOS, clinica_cantidad_pac(clinica, especialidad), especialidad);
		free(nombre);
	}

	if (!clinica_existe_doc(clinica, parametros[0])) {
		printf(ENOENT_DOCTOR, parametros[0]);
	} else if (!nombre) {
		printf(SIN_PACIENTES);
	}
}

// Función de visita para clinica_visitar_doc(), utilizada en informe_doctores()
// Cuenta la cantidad de doctores visitados, sumando uno a la variable cant cada vez que se visita.
void contar_doctores(const char* nombre, const char* especialidad, size_t pac_atendidos, void* cant) {
	(*((size_t*) cant))++;
}

// Función de visita para clinica_visitar_doc(), utilizada en informe_doctores()
// Imprime los doctores visitados con el mensaje del informe correspondiente.
void imprimir_doctores(const char* nombre, const char* especialidad, size_t pac_atendidos, void* indice) {
	printf(INFORME_DOCTOR, *((size_t*) indice), nombre, especialidad, pac_atendidos);
	(*((size_t*) indice))++;
}

void informe_doctores(const char** parametros, clinica_t* clinica) {
	const char* ini = parametros[0][0] == '\0' ? NULL : parametros[0];
	const char* fin = parametros[1][0] == '\0' ? NULL : parametros[1];

	size_t cantidad_doctores = 0;
	clinica_visitar_doc(clinica, contar_doctores, &cantidad_doctores, ini, fin);
	printf(DOCTORES_SISTEMA, cantidad_doctores);

	size_t indice = 1;
	clinica_visitar_doc(clinica, imprimir_doctores, &indice, ini, fin);
}
