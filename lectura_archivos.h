#ifndef __LECTURA_ARCHIVOS__H_
#define __LECTURA_ARCHIVOS__H_

#include "clinica.h"

// Carga los datos de todos los medicos y pacientes desde los archivos y los devuelve en una clinica.
// Si falla, imprime el mensaje de error y devuelve NULL.
clinica_t* cargar_datos(char* ruta_docs, char* ruta_pacs);

#endif
