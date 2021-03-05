#ifndef DOCTOR__H
#define DOCTOR__H
#include <stddef.h>

typedef struct doctor doctor_t;

// Crea un doctor con el nombre y especialidad indicados.
// Se debe destruir con doctor_destruir().
doctor_t* doctor_crear(const char* nombre, const char* especialidad);

// Destruye un doctor.
void doctor_destruir(doctor_t* doctor);

// Devuelve la cantidad de pacientes atendidos.
size_t doctor_cant_atendidos(doctor_t* doctor);

// Devuelve la especialidad del doctor.
const char* doctor_especialidad(doctor_t* doctor);

// Devuelve el nombre del doctor.
const char* doctor_nombre(doctor_t* doctor);

// Cuenta un nuevo paciente atendido, sumándo 1 al total.
void doctor_contar_paciente(doctor_t* doctor);

#endif // DOCTOR__H