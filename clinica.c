#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE 1
#define _XOPEN_SOURCE 500
#include "clinica.h"
#include "dependencias/hash.h"
#include "dependencias/abb.h"
#include "colapac.h"
#include "paciente.h"
#include <stdlib.h>
#include <string.h>

/*
  -------------- DEFINICION DE LAS ESTRUCTURAS  -------------- 
*/

struct clinica {
	colapac_t* colapac;
	hash_t* pacientes; // Diccionario de pacientes. Clave: nombre del paciente.
	abb_t* doctores; // Árbol de datos_doctor_t*. Clave: nombre del doctor.
};

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

// Wrapper de paciente_destruir() del TDA Paciente para usar en el Hash.
void paciente_destruir_wr(void* paciente) {
	paciente_destruir(paciente);
}

/*
  -------------- PRIMITIVAS TDA  -------------- 
*/

clinica_t* clinica_crear() {
	clinica_t* clinica = malloc(sizeof(clinica_t));
	if (!clinica) return NULL;

	clinica->colapac = colapac_crear();
	if (!clinica->colapac) {
		free(clinica);
		return NULL;
	}

	clinica->pacientes = hash_crear(paciente_destruir_wr);
	if (!clinica->pacientes) {
		colapac_destruir(clinica->colapac);
		free(clinica);
		return NULL;
	}

	clinica->doctores = abb_crear(strcmp, destruir_datos_doc);
	if (!clinica->doctores) {
		hash_destruir(clinica->pacientes);
		colapac_destruir(clinica->colapac);
		free(clinica);
		return NULL;
	}

	return clinica;
}

bool clinica_existe_pac(clinica_t* clinica, const char* paciente) {
	return hash_pertenece(clinica->pacientes, paciente);
}

bool clinica_existe_esp(clinica_t* clinica, const char* especialidad) {
	return colapac_existe(clinica->colapac, especialidad);
}

bool clinica_existe_doc(clinica_t* clinica, const char* doctor) {
	return abb_pertenece(clinica->doctores, doctor);
}

bool clinica_agregar_pac(clinica_t* clinica, const char* nombre, unsigned short anio_antig) {
	paciente_t* paciente = paciente_crear(nombre, anio_antig);
	if (!paciente) return false;

	if (!hash_guardar(clinica->pacientes, nombre, paciente)) {
		paciente_destruir(paciente);
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

	if (!colapac_agregar(clinica->colapac, especialidad)) {
		abb_borrar(clinica->doctores, nombre);
		destruir_datos_doc(datos);
		return false;
	}

	return true;
}

bool clinica_encolar(clinica_t* clinica, const char* nombre, const char* especialidad, bool urgente) {
	paciente_t* paciente = hash_obtener(clinica->pacientes, nombre);
	if (!paciente) return false;

	return colapac_encolar(clinica->colapac, paciente, especialidad, urgente);
}

void clinica_destruir(clinica_t* clinica) {
	colapac_destruir(clinica->colapac);
	hash_destruir(clinica->pacientes);
	abb_destruir(clinica->doctores);
	free(clinica);
}

size_t clinica_cantidad_pac(clinica_t* clinica, const char* especialidad) {
	return colapac_cantidad(clinica->colapac, especialidad);
}

const char* clinica_desencolar(clinica_t* clinica, const char* doctor) {
	datos_doctor_t* datos = abb_obtener(clinica->doctores, doctor);
	if (!datos) return NULL;

	paciente_t* paciente = colapac_desencolar(clinica->colapac, datos->especialidad);
	if (!paciente) return NULL;

	datos->pacientes_atendidos++;
	return paciente_nombre(paciente);
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
