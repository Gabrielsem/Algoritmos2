#ifndef CLINICA__H
#define CLINICA__H

#include <stddef.h>
#include <stdbool.h>

/* ******************************************************************
 *                    		 TDA CLINICA
 * *****************************************************************/

typedef struct clinica clinica_t;

// Función para clinica_visitar_doc()
typedef void (*visitar_doc_t)(const char* nombre, const char* especialidad, size_t pac_atendidos, void* extra);

// Crea una clinica, devuelve NULL si falla.
// Se debe destruir con clinica_destruir()
clinica_t* clinica_crear();

// Devuelve verdadero si el paciente existe, sino falso.
bool clinica_existe_pac(clinica_t* clinica, const char* paciente);

// Devuelve verdadero si la especialdiad existe, sino falso.
bool clinica_existe_esp(clinica_t* clinica, const char* especialidad);

// Devuelve verdadero si el doctor existe, sino falso.
bool clinica_existe_doc(clinica_t* clinica, const char* doctor);

// Agrega un paciente a los registros de la clinica.
// Devuelve verdadero si se logró, sino falso.
bool clinica_agregar_pac(clinica_t* clinica, const char* nombre, unsigned short anio_antig);

// Agrega un doctor a los registros de la clinica.
// Devuelve verdadero si se logró, sino falso.
bool clinica_agregar_doc(clinica_t* clinica, const char* nombre, const char* especialidad);

// Devuelve la cantidad de pacientes en espera para esa especialidad.
// Devuelve 0 si la especialidad no existe.
size_t clinica_cantidad_pac(clinica_t* clinica, const char* especialidad);

// Devuelve la especialidad de un doctor o NULL si este no existe.
const char* clinica_especialidad(clinica_t* clinica, const char* nombre);

// Visita todos los doctores entre inicio y fin (alfábeticamente) inclusive, con la función dada.
// Si inicio es NULL, imprime desde el primer doctor.
// Si fin es NULL, imprime desde el último doctor.
void clinica_visitar_doc(clinica_t* clinica, visitar_doc_t visitar, void* extra, const char* inicio, const char* fin);

// Destruye la clínica
// Cualquier paciente que haya quedado encolado al destruir se libera con free()
void clinica_destruir(clinica_t* clinica);

// Encola un paciente con la especialidad y urgencia dada.
// Devuelve false si falla (paciente o especialidad no existe o fallo de memoria),
// o true si se logra encolar.
bool clinica_encolar(clinica_t* clinica, const char* nombre_paciente, const char* especialidad, bool urgente);

// Desencola un paciente de la especialidad del doctor dado. Devuelve el nombre del paciente o NULL en caso
// de no haber pacientes encolados para esa especialidad o que no exista el doctor.
const char* clinica_desencolar(clinica_t* clinica, const char* doctor);

#endif // CLINICA__H