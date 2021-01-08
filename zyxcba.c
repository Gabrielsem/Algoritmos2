#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "funciones_tp2.h"
#include "dependencias/strutil.h"
#include "mensajes.h"
#include "csv.h"
#define _POSIX_C_SOURCE 200809L

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"
#define CANT_PARAMS_ARCHIVO 2

// Devuelve la cantidad de parámetros en el arreglo
size_t cant_params(const char** parametros) {
	size_t cant = 0;
	while (*parametros) {
		cant++;
		parametros++;
	}
	return cant;
}

void procesar_comando(const char* comando, const char** parametros, colapac_t* colapac, abb_t* doctores, hash_t* especialidades) {
	size_t cantidad = cant_params(parametros);
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
		if (cantidad == PARAMS_PEDIR_TURNO)
			pedir_turno(parametros, colapac, especialidades);
		else
			printf(ENOENT_PARAMS, comando);
	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {
		if (cantidad == PARAMS_ATENDER)
			atender_siguiente(parametros, colapac, doctores);
		else
			printf(ENOENT_PARAMS, comando);
	} else if (strcmp(comando, COMANDO_INFORME) == 0) {
		if (cantidad == PARAMS_INFORME)
			informe_doctores(parametros, doctores);
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

bool procesar_entrada(abb_t* doctores, hash_t* pacientes, hash_t* especialidades) {
	char* linea = NULL;
	size_t c = 0;
	colapac_t* colapac = colapac_crear(pacientes);
	if (!colapac) {
		printf(ERR_MEM);
		return false;
	}

	while (getline(&linea, &c, stdin) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, ':');
		if (campos[1] == NULL) {
			printf(ENOENT_FORMATO, linea);
			free_strv(campos);
			continue;	
		}
		char** parametros = split(campos[1], ',');
		procesar_comando(campos[0], (const char**) parametros, colapac, doctores, especialidades);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
	colapac_destruir(colapac);
	return true;
}

bool creador_abb(char** parametros, void* abb) {
	if(cant_params((const char**) parametros) != CANT_PARAMS_ARCHIVO) {
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
	if(cant_params((const char**) parametros) != CANT_PARAMS_ARCHIVO) {
		printf(ERR_PARAMS_ARCHIVO);
		return false;
	}
	char* fin;
	long int num_entrada = strtol(parametros[1], &fin, 10);
	if(*fin != '\0' || num_entrada <= 0 || num_entrada > USHRT_MAX){
		printf(ENOENT_ANIO, parametros[1]);
		return false;
	}
	unsigned short* antiguedad = malloc(sizeof(unsigned short));
	*antiguedad = (unsigned short) num_entrada;
	hash_guardar((hash_t*) hash, parametros[0], antiguedad);
	return true;
}

void destruir_datos_doc(void* datos_doc) {
	free(((datos_doctor_t*) datos_doc)->especialidad);
	free(datos_doc);
}

abb_t* leer_doctores(char* ruta) {
	abb_t* doctores = abb_crear(strcmp, destruir_datos_doc);
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

bool visitar_doctores(const char* nombre, void* datos_doc, void* especialidades) {
	if (hash_pertenece((hash_t*) especialidades, ((datos_doctor_t*) datos_doc)->especialidad)) return true;
	hash_guardar((hash_t*) especialidades, ((datos_doctor_t*) datos_doc)->especialidad, NULL);
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
	if (!doctores)
		return 1;
	hash_t* pacientes = leer_pacientes(argv[2]);
	if (!pacientes) {
		printf(ERR_MEM);
		abb_destruir(doctores);
		return 1;
	}
	hash_t* especialidades = obtener_especialidades(doctores);
	if (!especialidades) {
		printf(ERR_MEM);
		hash_destruir(pacientes);
		abb_destruir(doctores);
		return 1;
	}
	if (!procesar_entrada(doctores, pacientes, especialidades)) {
		hash_destruir(pacientes);
		abb_destruir(doctores);
		hash_destruir(especialidades);
		return 1;
	}
	hash_destruir(pacientes);
	abb_destruir(doctores);
	hash_destruir(especialidades);
	return 0;
}
