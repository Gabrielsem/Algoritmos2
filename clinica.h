#ifndef CLINICA__H
#define CLINICA__H

#include <stddef.h>
#include <stdbool.h>
#include "dependencias/hash.h"

/* ******************************************************************
 *                    TDA COLA DE PACIENTES
 * *****************************************************************/

typedef struct clinica clinica_t;

// Crea una cola de pacientes, devuelve NULL si falla.
// Recibe un diccionario con las antiguedades de cada paciente (clave: nombre, dato: año (unsigned short*))
// No modificar el diccionario tras crear una cola de pacientes
// Se debe destruir con clinica_destruir()
clinica_t* clinica_crear(hash_t* antiguedades);

// Devuelve si el paciente existe (esta en el diccionario de antiguedades)
bool clinica_existe_pac(clinica_t* clinica, const char* paciente);

// Destruye una cola de pacientes
// No destruye el diccionario recibido en clinica_crear().
// Cualquier paciente que quede encolado al destruir se libera con free()
void clinica_destruir(clinica_t* clinica);

// Encola un paciente con la especialidad y urgencia dada.
// Devuelve false si falla (paciente no existe en diccionario o fallo de memoria),
// o true si se logra encolar.
// Pre: Paciente y especialidad existen
bool clinica_encolar(clinica_t* clinica, char* paciente, const char* especialidad, bool urgente);

// Devuelve la cantidad de pacientes en espera para esa especialidad.
size_t clinica_cantidad(clinica_t* clinica, const char* especialidad);

// Desencola un paciente de la especialidad dada. Devuelve el nombre del paciente o NULL en caso
// de no haber pacientes encolados para esa especialidad.
char* clinica_desencolar(clinica_t* clinica, const char* especialidad);

#endif // CLINICA__H