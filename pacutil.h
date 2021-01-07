#ifndef PACUTIL__H
#define PACUTIL__H

#include <stddef.h>
#include <stdbool.h>

/* ******************************************************************
 *                        TDA PACIENTE
 * *****************************************************************/

typedef struct paciente paciente_t;

// Crea un paciente y lo devuelve. No se almacena el puntero de nombre pasado.
// Se debe destruir con destruir_paciente()
paciente_t* crear_paciente(const char* nombre, unsigned short anio_ant);

// Devuelve el nombre del paciente.
const char* nombre_paciente(paciente_t* paciente);

// Destruye un paciente, liberando la memoria pedida.
// Recibe un void* para facilitar su uso con otros TDAs, pero debe ser
// un paciente_t* previamente creado con crear_paciente().
void destruir_paciente(void* paciente);

// Compara la antiguedad de dos pacientes. Devuelve:
// 0 si es la misma antiguedad
// > 0 si el paciente 1 tiene mas antiguedad que el paciente 2
// < 0 si el paciente 1 tiene menos antiguedad que el paciente 2
int antiguedad_cmp(const paciente_t* paciente_1, const paciente_t* paciente_2);

/* ******************************************************************
 *                    TDA COLA DE PACIENTES
 * *****************************************************************/

typedef struct colapac colapac_t;

// Crea una cola de pacientes, devuelve NULL si falla.
// Se debe destruir con colapac_destruir()
colapac_t* colapac_crear(void);

// Destruye una cola de pacientes y todos los pacientes encolados
void colapac_destruir(colapac_t* colapac);

// Encola un paciente con la especialidad y urgencia dada. Devuelve false si falla,
// true en caso contrario.
bool colapac_encolar(colapac_t* colapac, paciente_t* paciente, const char* especialidad, bool urgente);

// Devuelve la cantidad de pacientes en espera para esa especialidad.
size_t colapac_cantidad(colapac_t* colapac, const char* especialidad);

// Desencola un paciente de la especialidad dada. Devuelve el paciente o NULL en caso
// de no haber pacientes encolados para esa especialidad.
paciente_t* colapac_desencolar(colapac_t* colapac, const char* especialidad);

#endif // PACUTIL__H