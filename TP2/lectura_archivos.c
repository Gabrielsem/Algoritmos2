#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE 1
#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "lectura_archivos.h"
#include "funciones_tp2.h"
#include "mensajes.h"
#include "dependencias/strutil.h"

#define SEPARADOR ','
#define CANT_PARAMS_ARCHIVO 2

static void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

// Devuelve la cantidad de strings en el arreglo
// Debe terminar en NULL
static size_t cant_params(const char** parametros) {
	size_t cant = 0;
	while (*parametros) {
		cant++;
		parametros++;
	}
	return cant;
}

/**
Haciendo uso de strutil (split) lee un archivo csv y, línea a línea,
va llamando al constructor que se pasa por parámetro. Dicho constructor se invoca
con la línea separada por split, sin fines de línea ni ninguna otra consideración,
y con el puntero extra que se pasa por parámetro.
Importante: la función constructor no debe guardar las referencias a las cadenas
dentro de arreglo de cadenas pasado por parámetros (hacer copias en caso de ser 
necesario); luego de invocarse el constructor se invoca a free_strv.

Devuelve verdadero si se logró o falso si falló algún creador o en caso que el archivo
csv (indicado por la ruta pasada por parámetro) no exista. 
**/
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

// -------------- DOCTORES ---------------


bool creador_docs(char** parametros, void* clinica) {
	if (cant_params((const char**) parametros) != CANT_PARAMS_ARCHIVO) {
		printf(ERR_PARAMS_ARCHIVO);
		return false;
	}

	if (!clinica_agregar_doc((clinica_t*) clinica, parametros[0], parametros[1])) {
		printf(ERR_MEM);
		return false;
	}

	return true;
}

// ------------- PACIENTES ---------------


bool creador_pacs(char** parametros, void* clinica) {
	if (cant_params((const char**) parametros) != CANT_PARAMS_ARCHIVO) {
		printf(ERR_PARAMS_ARCHIVO);
		return false;
	}

	char* fin;
	long int num_entrada = strtol(parametros[1], &fin, 10);
	if (*fin != '\0' || num_entrada <= 0 || num_entrada > USHRT_MAX) {
		printf(ENOENT_ANIO, parametros[1]);
		return false;
	}

	if (!clinica_agregar_pac((clinica_t*) clinica, parametros[0], (unsigned short) num_entrada)) {
		printf(ERR_MEM);
		return false;
	}

	return true;
}

// ---------------------------------------

clinica_t* cargar_datos(char* ruta_docs, char* ruta_pacs) {
	clinica_t* clinica = clinica_crear();
	if (!clinica) return NULL;

	if(csv_crear_estructura(ruta_docs, creador_docs, clinica) && csv_crear_estructura(ruta_pacs, creador_pacs, clinica))
		return clinica;
	
	clinica_destruir(clinica);
	return NULL;
}