#include "clinica.h"
#include "dependencias/heap.h"
#include "dependencias/cola.h"
#include "dependencias/hash.h"
#include "dependencias/abb.h"
#include <stdlib.h>
#include <string.h>
#define _POSIX_C_SOURCE 200809L

/*
  -------------- DEFINICION DE LAS ESTRUCTURAS  -------------- 
*/

typedef struct paciente {
	char* nombre;
	unsigned short anio_ant;
} paciente_t;

struct clinica {
	hash_t* colas; // Diccionario de colaesp_t*. Clave: especialidad.
	hash_t* antiguedades; // Diccionario de año de antiguedad de pacientes (unsigned short). Clave: nombre del paciente.
	abb_t* doctores; // Árbol de datos_doctor_t*. Clave: nombre del doctor.
};

// cola de una especialidad
typedef struct colaesp {
	cola_t* urgentes;
	heap_t* regulares;
	size_t cant_en_espera;
} colaesp_t;

typedef struct doctor {
	char* especialidad;
	size_t pacientes_atendidos;
} datos_doctor_t;

// Para pasar mas de un extra a función de iterador de abb en clinica_visitar_doc()
typedef struct extras_visitar {
	visitar_doc_t func_visitar;
	void* extra_real;
} extras_t;

/*
  -------------- FUNCIONES INTERNAS  -------------- 
*/

// Crea un paciente con el nombre y año de antiguedad dados
// Devuelve NULL si falla
paciente_t* crear_paciente(char* nombre, unsigned short anio_ant) {
	paciente_t* paciente = malloc(sizeof(paciente_t));
	if (!paciente) return NULL;

	paciente->nombre = nombre;
	paciente->anio_ant = anio_ant;
	return paciente;
}

// Compara la antiguedad de dos pacientes. Devuelve:
// 0 si es la misma antiguedad
// > 0 si el paciente 1 tiene mas antiguedad que el paciente 2
// < 0 si el paciente 1 tiene menos antiguedad que el paciente 2
int antiguedad_cmp(const void* paciente_1, const void* paciente_2) {
	int anio_1 = ((paciente_t*) paciente_1)->anio_ant;
	int anio_2 = ((paciente_t*) paciente_2)->anio_ant;
	return anio_2 - anio_1;
}

// Destruye un paciente, liberando también el nombre almacenado
void destruir_paciente(void* paciente) {
	free(((paciente_t*) paciente)->nombre);
	free(paciente);
}

// Destruye una cola de una especialidad y todos sus pacientes, si es que la cola existe
void destruir_colaesp(void* colaesp) {
	if (!colaesp) return;

	cola_destruir(((colaesp_t*) colaesp)->urgentes, free);
	heap_destruir(((colaesp_t*) colaesp)->regulares, destruir_paciente);
	free(colaesp);
}

// Crea una cola para una especialidad y la devuelve. Si falla la creación,
// devuelve NULL
colaesp_t* crear_colaesp() {
	colaesp_t* colaesp = malloc(sizeof(colaesp_t));
	if (!colaesp) return NULL;

	colaesp->urgentes = cola_crear();
	if (!colaesp->urgentes) {
		free(colaesp);
		return NULL;
	}

	colaesp->regulares = heap_crear(antiguedad_cmp);
	if (!colaesp->regulares) {
		cola_destruir(colaesp->urgentes, NULL);
		free(colaesp);
		return NULL;
	}

	colaesp->cant_en_espera = 0;
	return colaesp;
}

// Destruye los datos de un doctor
void destruir_datos_doc(void* datos_doc) {
	free(((datos_doctor_t*) datos_doc)->especialidad);
	free(datos_doc);
}

// Crea los datos de un doctor con esa especialidad, inicializando los pacientes atendidos en 0
datos_doctor_t* crear_datos_doc(const char* especialidad) {
	datos_doctor_t* datos = malloc(sizeof(datos_doctor_t));
	if (!datos) return NULL;

	datos->especialidad = strdup(especialidad);
	if (!datos->especialidad) {
		free(datos);
		return false;
	}

	datos->pacientes_atendidos = 0;
	return datos;
}

// Función de visita para el TDA ABB utilizada en clinica_visitar_doc()
// Es un wrapper de la función de visita del usuario del TDA
void visitar_aux(const char* nombre, void* datos_doc, void* extra) {
	extras_t* extras = extra;
	datos_doctor_t* datos = datos_doc;
	extras->func_visitar(nombre, datos->especialidad, datos->pacientes_atendidos, extras->extra_real);
}

/*
  -------------- PRIMITIVAS TDA  -------------- 
*/

clinica_t* clinica_crear() {
	clinica_t* clinica = malloc(sizeof(clinica_t));
	if (!clinica) return NULL;

	clinica->colas = hash_crear(destruir_colaesp);
	if (!clinica->colas) {
		free(clinica);
		return NULL;
	}

	clinica->antiguedades = hash_crear(free);
	if (!clinica->antiguedades) {
		hash_destruir(clinica->colas);
		free(clinica);
		return NULL;
	}

	clinica->doctores = abb_crear(strcmp, destruir_datos_doc);
	if (!clinica->doctores) {
		hash_destruir(clinica->antiguedades);
		hash_destruir(clinica->colas);
		free(clinica);
		return NULL;
	}

	return clinica;
}

bool clinica_existe_pac(clinica_t* clinica, const char* paciente) {
	return hash_pertenece(clinica->antiguedades, paciente);
}

bool clinica_existe_esp(clinica_t* clinica, const char* especialidad) {
	return hash_pertenece(clinica->colas, especialidad);
}

bool clinica_existe_doc(clinica_t* clinica, const char* doctor) {
	return abb_pertenece(clinica->doctores, doctor);
}

bool clinica_agregar_pac(clinica_t* clinica, const char* nombre, unsigned short anio_antig) {
	unsigned short* anio = malloc(sizeof(unsigned short));
	if (!anio) return false;
	*anio = anio_antig;

	if (!hash_guardar(clinica->antiguedades, nombre, anio)) {
		free(anio);
		return false;
	}
	return true;
}

bool clinica_agregar_doc(clinica_t* clinica, const char* nombre, const char* especialidad) {
	datos_doctor_t* datos = crear_datos_doc(especialidad);
	if (!datos) return false;

	if (!abb_guardar(clinica->doctores, nombre, datos)) {
		destruir_datos_doc(datos);
		return false;
	}

	if(hash_pertenece(clinica->colas, especialidad))
		return true;

	if (!hash_guardar(clinica->colas, especialidad, NULL)) {
		abb_borrar(clinica->doctores, nombre);
		destruir_datos_doc(datos);
		return false;
	}

	return true;
}

bool clinica_encolar(clinica_t* clinica, char* nombre, const char* especialidad, bool urgente) {
	unsigned short* anio = hash_obtener(clinica->antiguedades, nombre);
	if (!anio) return false;

	if (!hash_pertenece(clinica->colas, especialidad)) return false;

	colaesp_t* colaesp = hash_obtener(clinica->colas, especialidad);
	if (!colaesp) {
		colaesp = crear_colaesp();
		if (!colaesp) return false;

		if (!hash_guardar(clinica->colas, especialidad, colaesp)) {
			destruir_colaesp(colaesp);
			return false;
		}
	}

	bool encolo = false;
	if (urgente) {
		encolo = cola_encolar(colaesp->urgentes, nombre);
	} else {
		paciente_t* paciente = crear_paciente(nombre, *anio);
		if (!paciente) return false;
		encolo = heap_encolar(colaesp->regulares, paciente);
	}

	if (encolo) colaesp->cant_en_espera++;
	return encolo;
}

void clinica_destruir(clinica_t* clinica) {
	hash_destruir(clinica->colas);
	hash_destruir(clinica->antiguedades);
	abb_destruir(clinica->doctores);
	free(clinica);
}

size_t clinica_cantidad_pac(clinica_t* clinica, const char* especialidad) {
	colaesp_t* colaesp = hash_obtener(clinica->colas, especialidad);
	if (!colaesp) {
		return 0;
	}

	return colaesp->cant_en_espera;
}

char* clinica_desencolar(clinica_t* clinica, const char* doctor) {
	datos_doctor_t* datos = abb_obtener(clinica->doctores, doctor);
	if (!datos) return NULL;

	colaesp_t* colaesp = hash_obtener(clinica->colas, datos->especialidad);
	if (!colaesp) return NULL;

	char* nombre = cola_desencolar(colaesp->urgentes);
	if(!nombre) {
		paciente_t* paciente = heap_desencolar(colaesp->regulares);
		if (!paciente) return NULL;

		nombre = paciente->nombre;
		free(paciente);
	}

	colaesp->cant_en_espera--;
	datos->pacientes_atendidos++;
	return nombre;
}

const char* clinica_especialidad(clinica_t* clinica, const char* doctor) {
	datos_doctor_t* datos = abb_obtener(clinica->doctores, doctor);
	return datos ? datos->especialidad : NULL;
}

void clinica_visitar_doc(clinica_t* clinica, visitar_doc_t visitar, void* extra, const char* inicio, const char* fin) {
	extras_t extras;
	extras.func_visitar = visitar;
	extras.extra_real = extra;
	abb_in_order_rng(clinica->doctores, visitar_aux, &extras, inicio, fin);
}
