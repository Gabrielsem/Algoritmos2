#ifndef FUNCTP2__H
#define FUNCTP2__H
#include "colapac.h"
#include "hash.h"
#include "abb.h"

#define PARAMS_PEDIR_TURNO 3
#define PARAMS_ATENDER 1
#define PARAMS_INFORME 2

typedef struct doctor {
	char* especialidad;
	size_t pacientes_atendidos;
} datos_doctor_t;

// Pre: Debe haber PARAMS_PEDIR_TURNO parámetros.
void pedir_turno(const char** parametros, colapac_t* colapac, hash_t* especialidades);

// Pre: Debe haber PARAMS_ATENDER parámetros. El árbol de doctores es de datos_doctor_t*
void atender_siguiente(const char** parametros, colapac_t* colapac, abb_t* doctores);

// Pre: Debe haber PARAMS_INFORME parámetros. El árbol de doctores es de datos_doctor_t*
void informe_doctores(const char** parametros, abb_t* doctores);

#endif // FUNCTP2_H