#ifndef PACUTIL__H
#define PACUTIL__H

#include <stddef.h>
#include <stdbool.h>
#include "hash.h"

/* ******************************************************************
 *                    TDA COLA DE PACIENTES
 * *****************************************************************/

typedef struct colapac colapac_t;

// Crea una cola de pacientes, devuelve NULL si falla.
// Recibe un diccionario con las antiguedades de cada paciente (clave: nombre, dato: año (short int*))
// No modificar el diccionario tras crear una cola de pacientes
// Se debe destruir con colapac_destruir()
colapac_t* colapac_crear(hash_t* antiguedades);

// Destruye una cola de pacientes
// No destruye el diccionario
void colapac_destruir(colapac_t* colapac);

// Encola un paciente con la especialidad y urgencia dada.
// Devuelve false si falla (paciente no existe en diccionario o fallo de memoria),
// o true si se logra encolar.
bool colapac_encolar(colapac_t* colapac, char* paciente, const char* especialidad, bool urgente);

// Devuelve la cantidad de pacientes en espera para esa especialidad.
size_t colapac_cantidad(colapac_t* colapac, const char* especialidad);

// Desencola un paciente de la especialidad dada. Devuelve el nombre del paciente o NULL en caso
// de no haber pacientes encolados para esa especialidad.
char* colapac_desencolar(colapac_t* colapac, const char* especialidad);

#endif // PACUTIL__H