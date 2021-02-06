#include <stdio.h>
#include <string.h>
#include "funciones_tp2.h"
#include "dependencias/strutil.h"
#include "mensajes.h"
#include "lectura_archivos.h"
#include "stdlib.h"
#define _POSIX_C_SOURCE 200809L

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"

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

// Procesa el comando dado, imprimiendo el mensaje de error correspondiente si falla.
void procesar_comando(const char* comando, const char** parametros, clinica_t* clinica) {
	size_t cantidad = cant_params(parametros);
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {

		if (cantidad == PARAMS_PEDIR_TURNO)
			pedir_turno(parametros, clinica);
		else
			printf(ENOENT_PARAMS, comando);

	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {

		if (cantidad == PARAMS_ATENDER)
			atender_siguiente(parametros, clinica);
		else
			printf(ENOENT_PARAMS, comando);

	} else if (strcmp(comando, COMANDO_INFORME) == 0) {

		if (cantidad == PARAMS_INFORME)
			informe_doctores(parametros, clinica);
		else
			printf(ENOENT_PARAMS, comando);

	} else {
		printf(ENOENT_CMD, comando);
	}
}

int main(int argc, char** argv) {
	if (argc != 3) {
		printf(ENOENT_CANT_PARAMS);
		return 1;
	}

	clinica_t* clinica = cargar_datos(argv[1], argv[2]);
	if (!clinica) return 1;

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
		procesar_comando(campos[0], (const char**) parametros, clinica);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
	clinica_destruir(clinica);
	return 0;
}
