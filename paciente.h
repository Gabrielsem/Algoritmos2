#ifndef PACIENTE__H
#define PACIENTE__H

/* ******************************************************************
 *                     		TDA PACIENTE
 * *****************************************************************/

typedef struct paciente paciente_t;

// Crea un paciente con el nombre y año de antiguedad dados
// Devuelve NULL si falla. Se debe destruir con paciente_destruir().
paciente_t* paciente_crear(const char* nombre, unsigned short anio_ant);

// Devuelve el nombre del paciente.
const char* paciente_nombre(paciente_t* paciente);

// Destruye el paciente.
void paciente_destruir(paciente_t* paciente);

// Compara la antiguedad de dos pacientes. Devuelve:
// 0 si es la misma antiguedad
// > 0 si el paciente 1 tiene mas antiguedad que el paciente 2
// < 0 si el paciente 1 tiene menos antiguedad que el paciente 2
int paciente_antiguedad_cmp(const paciente_t* paciente_1, const paciente_t* paciente_2);

#endif // PACIENTE__H