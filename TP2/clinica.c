#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE 1
#define _XOPEN_SOURCE 500
#include "clinica.h"
#include "dependencias/hash.h"
#include "dependencias/abb.h"
#include "colapac.h"
#include "paciente.h"
#include "doctor.h"
#include <stdlib.h>
#include <string.h>

/*
  -------------- DEFINICION DE LAS ESTRUCTURAS  -------------- 
*/

struct clinica {
	colapac_t* colapac; // Cola de pacientes de todas las especialidades.
	hash_t* pacientes; // Diccionario de pacientes. Clave: nombre del paciente.
	abb_t* doctores; // Árbol de doctor_t*. Clave: nombre del doctor.
};

// Para pasar mas de un extra a función de iterador de abb en clinica_visitar_doc()
typedef struct extras_visitar {
	visitar_doc_t func_visitar;
	void* extra_real;
} extras_t;

/*
  -------------- FUNCIONES INTERNAS  -------------- 
*/

// Función de visita para el TDA ABB utilizada en clinica_visitar_doc()
// Es un wrapper de la función de visita del usuario del TDA
void visitar_aux(const char* nombre, void* doctor, void* extra) {
	extras_t* extras = extra;
	doctor_t* doc = doctor;
	extras->func_visitar(doctor_nombre(doc), doctor_especialidad(doc), doctor_cant_atendidos(doc), extras->extra_real);
}

// Wrapper de paciente_destruir() del TDA Paciente para usar en el Hash.
void paciente_destruir_wr(void* paciente) {
	paciente_destruir(paciente);
}

// Wrapper de doctor_destruir() del TDA Doctor para usar en el ABB.
void doctor_destruir_wr(void* doctor) {
	doctor_destruir(doctor);
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

	clinica->doctores = abb_crear(strcmp, doctor_destruir_wr);
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
	doctor_t* doctor = doctor_crear(nombre, especialidad);
	if (!doctor) return false;

	if (!abb_guardar(clinica->doctores, nombre, doctor)) {
		doctor_destruir(doctor);
		return false;
	}

	if (!colapac_agregar(clinica->colapac, especialidad)) {
		abb_borrar(clinica->doctores, nombre);
		doctor_destruir(doctor);
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

const char* clinica_desencolar(clinica_t* clinica, const char* nombre_doc) {
	doctor_t* doctor = abb_obtener(clinica->doctores, nombre_doc);
	if (!doctor) return NULL;

	paciente_t* paciente = colapac_desencolar(clinica->colapac, doctor_especialidad(doctor));
	if (!paciente) return NULL;

	doctor_contar_paciente(doctor);
	return paciente_nombre(paciente);
}

const char* clinica_especialidad(clinica_t* clinica, const char* nombre_doc) {
	doctor_t* doctor = abb_obtener(clinica->doctores, nombre_doc);
	return doctor ? doctor_especialidad(doctor) : NULL;
}

void clinica_visitar_doc(clinica_t* clinica, visitar_doc_t visitar, void* extra, const char* inicio, const char* fin) {
	extras_t extras;
	extras.func_visitar = visitar;
	extras.extra_real = extra;
	abb_in_order_rng(clinica->doctores, visitar_aux, &extras, inicio, fin);
}
