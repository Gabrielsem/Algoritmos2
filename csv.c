#define _POSIX_C_SOURCE 200809L //getline
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv.h"
#define SEPARADOR ','

static void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

void csv_crear_estructura(const char* ruta_csv, void* (*creador) (char**, void*), void* extra) {
	FILE* archivo = fopen(ruta_csv, "r");
	if (!archivo) {
		return NULL;
	}
	
	if (!extra) {
		fclose(archivo);
		return NULL;
	}

	char* linea = NULL;
	size_t c = 0;
	while (getline(&linea, &c, archivo) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, SEPARADOR);
		creador(campos, extra);
		free_strv(campos);
	}
	free(linea);
	fclose(archivo);
}

void creador_abb(char** parametros, void* abb) {
	abb_guardar((abb_t*) abb, parametros[1], parametros[2]);
}

void creador_hash(char** parametros, void* hash) {
	hash_guardar((hash_t*) hash, parametros[1],atoi(parametros[2]));
}