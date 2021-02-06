#ifndef FUNCTP2__H
#define FUNCTP2__H
#include "clinica.h"

#define PARAMS_PEDIR_TURNO 3
#define PARAMS_ATENDER 1
#define PARAMS_INFORME 2

typedef struct doctor {
	char* especialidad;
	size_t pacientes_atendidos;
} datos_doctor_t;

// Funciones para cada comando de pedir turno, con mensajes de error.

// Pre: Debe haber PARAMS_PEDIR_TURNO parámetros.
void pedir_turno(const char** parametros, clinica_t* clinica);

// Pre: Debe haber PARAMS_ATENDER parámetros.
void atender_siguiente(const char** parametros, clinica_t* clinica);

// Pre: Debe haber PARAMS_INFORME parámetros.
void informe_doctores(const char** parametros, clinica_t* clinica);

#endif // FUNCTP2_H