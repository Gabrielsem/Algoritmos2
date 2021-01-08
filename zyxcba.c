#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "funciones_tp2.h"
#include "strutil.h"
#include "mensajes.h"
#include "csv.h"
#define _POSIX_C_SOURCE 200809L

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"

// Devuelve la cantidad de parámetros en el arreglo
size_t cant_params(const char** parametros) {
	size_t cant = 0;
	while (*parametros) {
		cant++;
		parametros++;
	}
	return cant;
}

void procesar_comando(const char* comando, const char** parametros) {
	size_t cantidad = cant_params(parametros);
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
		if (cantidad == PARAMS_PEDIR_TURNO)
			pedir_turno(parametros);
		else
			printf(ENOENT_PARAMS, comando);
	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {
		if (cantidad == PARAMS_ATENDER)
			atender_siguiente(parametros);
		else
			printf(ENOENT_PARAMS, comando);
	} else if (strcmp(comando, COMANDO_INFORME) == 0) {
		if (cantidad == PARAMS_INFORME)
			informe_doctores(parametros);
		else
			printf(ENOENT_PARAMS, comando);
	} else {
		printf(ENOENT_CMD, comando);
	}
}

void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

void procesar_entrada() {
	char* linea = NULL;
	size_t c = 0;
	while (getline(&linea, &c, stdin) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, ':');
		if (campos[1] == NULL) {
			printf(ENOENT_FORMATO, linea);
			free_strv(campos);
			continue;	
		}
		char** parametros = split(campos[1], ',');
		procesar_comando(campos[0], (const char**) parametros);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}

bool creador_abb(char** parametros, void* abb) {
	if(cant_params(parametros) != 2) {
		printf(ERR_PARAMS_ARCHIVO);
		return false;
	}
	void* especialidad = strdup(parametros[1]);
	if(!especialidad) {
		printf(ERR_MEM);
		return false;
	}
	datos_doctor_t* datos = malloc(sizeof(datos_doctor_t));
	datos->especialidad = especialidad;
	datos->pacientes_atendidos = 0;
	bool guardo = abb_guardar((abb_t*) abb, parametros[0], datos);
	if(!guardo) {
		free(especialidad);
		free(datos);
		printf(ERR_MEM);
		return false;
	}
	return true;
}

bool creador_hash(char** parametros, void* hash) {
	char* fin;
	long int num_entrada = strtol(parametros[1], &fin, 10);
	if(*fin != '\0' || num_entrada <= 0 || num_entrada > USHRT_MAX){
		printf(ENOENT_ANIO, parametros[1]);
		return false;
	}
	unsigned short* antiguedad = malloc(sizeof(unsigned short));
	*antiguedad = (unsigned short) num_entrada;
	hash_guardar((hash_t*) hash, parametros[0],antiguedad);
	return true;
}

abb_t* leer_doctores(char* ruta) {
	abb_t* doctores = abb_crear(strcmp, free);
	if(!csv_crear_estructura(ruta, creador_abb, doctores)) {
		abb_destruir(doctores);
		return NULL;
	}
	return doctores;
}

hash_t* leer_pacientes(char* ruta) {
	hash_t* pacientes = hash_crear(free);
	if(!csv_crear_estructura(ruta, creador_hash, pacientes)) {
		hash_destruir(pacientes);
		return NULL;
	}
	return pacientes;
}

bool visitar_doctores(char* nombre, void* especialidad,void* especialidades) {
	if(hash_pertenece((const hash_t *)especialidades, (const char *)nombre)) return true;
	hash_guardar((hash_t *)especialidades, (const char *)especialidad, NULL);
	return true;
}

hash_t* obtener_especialidades(abb_t* doctores) {
	hash_t* especialidades = hash_crear(NULL);
	abb_in_order(doctores, visitar_doctores, especialidades);
	return especialidades;
}

int main(int argc, char** argv) {
	if(argc != 3) {
		printf(ENOENT_CANT_PARAMS);
		return 0;
	}
	abb_t* doctores = leer_doctores(argv[1]);
	hash_t* pacientes = leer_pacientes(argv[2]);
	if(!doctores || !pacientes){
		return 1;
	}
	hash_t* especialidades = obtener_especialidades(doctores);
	procesar_entrada();
	return 0;
}
