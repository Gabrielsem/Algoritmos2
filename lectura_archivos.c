#define _POSIX_C_SOURCE 200809L //getline
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "lectura_archivos.h"
#include "funciones_tp2.h"
#include "mensajes.h"
#define SEPARADOR ','

#define CANT_PARAMS_ARCHIVO 2

static void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

// Devuelve la cantidad de parámetros en el arreglo
size_t cant_params(const char** parametros) {
	size_t cant = 0;
	while (*parametros) {
		cant++;
		parametros++;
	}
	return cant;
}

bool csv_crear_estructura(const char* ruta_csv, bool (*creador) (char**, void*), void* extra) {
	FILE* archivo = fopen(ruta_csv, "r");
	if (!archivo) {
		printf(ENOENT_ARCHIVO, ruta_csv);
		return false;
	}
	
	char* linea = NULL;
	size_t c = 0;
	while (getline(&linea, &c, archivo) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, SEPARADOR);
		if(!creador(campos, extra)){
			free_strv(campos);
			free(linea);
			fclose(archivo);
			return false;
		}
		free_strv(campos);
	}
	free(linea);
	fclose(archivo);
	return true;
}

bool creador_abb(char** parametros, void* abb) {
	if (cant_params((const char**) parametros) != CANT_PARAMS_ARCHIVO) {
		printf(ERR_PARAMS_ARCHIVO);
		return false;
	}

	void* especialidad = strdup(parametros[1]);
	if (!especialidad) {
		printf(ERR_MEM);
		return false;
	}
	datos_doctor_t* datos = malloc(sizeof(datos_doctor_t));
	datos->especialidad = especialidad;
	datos->pacientes_atendidos = 0;
	bool guardo = abb_guardar((abb_t*) abb, parametros[0], datos);
	if (!guardo) {
		free(especialidad);
		free(datos);
		printf(ERR_MEM);
		return false;
	}
	return true;
}

bool creador_hash(char** parametros, void* hash) {
	if (cant_params((const char**) parametros) != CANT_PARAMS_ARCHIVO) {
		printf(ERR_PARAMS_ARCHIVO);
		return false;
	}

	char* fin;
	long int num_entrada = strtol(parametros[1], &fin, 10);
	if (*fin != '\0' || num_entrada <= 0 || num_entrada > USHRT_MAX){
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
	if (!doctores) {
		printf(ERR_MEM);
		return NULL;
	}
	
	if (!csv_crear_estructura(ruta, creador_abb, doctores)) {
		abb_destruir(doctores);
		return NULL;
	}
	return doctores;
}

hash_t* leer_pacientes(char* ruta) {
	hash_t* pacientes = hash_crear(free);
	if (!pacientes) {
		printf(ERR_MEM);
		return NULL;
	}

	if (!csv_crear_estructura(ruta, creador_hash, pacientes)) {
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
	if (!especialidades) {
		printf(ERR_MEM);
		return NULL;
	}

	abb_in_order(doctores, visitar_doctores, especialidades);
	return especialidades;
}

bool cargar_datos(abb_t** doctores, hash_t** pacientes, hash_t** especialidades, char* ruta_docs, char* ruta_pacs) {
	*doctores = leer_doctores(ruta_docs);
	if (!*doctores){
		return false;
	}
	*pacientes = leer_pacientes(ruta_pacs);
	if (!*pacientes) {
		abb_destruir(*doctores);
		return false;
	}
	*especialidades = obtener_especialidades(*doctores);
	if (!*especialidades) {
		hash_destruir(*pacientes);
		abb_destruir(*doctores);
		return false;
	}
	return true;
}
