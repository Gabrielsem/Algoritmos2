#ifndef COLAPAC__H
#define COLAPAC__H

#include <stddef.h>
#include <stdbool.h>

// TDA Cola de Pacientes

typedef struct colapac colapac_t;

colapac_t* colapac_crear(void);

void colapac_destruir(colapac_t* colapac);

void colapac_encolar(colapac_t* colapac, char* nombre, const char* especialidad, bool urgente);

size_t colapac_cantidad(colapac_t* colapac);

// Devuelve el nombre del paciente
char* colapac_desencolar(colapac_t* colapac, const char* especialidad);

#endif // COLAPAC__H