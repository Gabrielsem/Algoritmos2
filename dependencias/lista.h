#ifndef LISTA_H
#define LISTA_H

#include <stddef.h>
#include <stdbool.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* La lista está planteada como una lista de punteros genéricos. */
typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

//Post: Devuelve una nueva lista vacía o NULL si no se pudo crear.
lista_t *lista_crear(void);

// Pre: La lista fue creada.
// Post: Devuelve true si la lista no tiene elementos, sino false.
bool lista_esta_vacia(const lista_t *lista);

// Pre: La lista fue creada.
// Post: se agregó un nuevo elemento a la lista con el dato en la primera
// posición de la lista y devuelve true, o false en caso de error.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Pre: La lista fue creada.
// Post: Se agregó un nuevo elemento a la lista con el dato en la última
// posición de la lista y devuelve true. Falso en caso de error.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Pre: La lista fue creada.
// Post: Se borra el primer elemento de la lista y
// se devuelve su valor. Si está vacía, devuelve NULL.
void *lista_borrar_primero(lista_t *lista);


// Pre: La lista fue creada.
// Post: Devuelve el valor del primer elemento
// de la lista. Si está vacía, devuelve NULL.
void *lista_ver_primero(const lista_t *lista);

// Pre: La lista fue creada.
// Post: Devuelve el valor del último elemento
// de la lista. Si está vacía, devuelve NULL.
void *lista_ver_ultimo(const lista_t* lista);

//Pre: La lista fue creada.
//Post: Devuelve la cantidad de elementos de la lista.
size_t lista_largo(const lista_t *lista);

// Pre: La lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

/* ******************************************************************
 *               PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

// Pre: La lista fue creada.
// Post: Recorre los elementos de la lista, aplicándoles la función visitar.
// Se deja de recorrer la lista si se llega al final o visitar devuelve false.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

/* ******************************************************************
 *               PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

// Pre: La lista fue creada.
// Post: Devuelve un nuevo iterador al inicio de la lista o
// NULL si no se pudo crear.
lista_iter_t *lista_iter_crear(lista_t *lista);

// Pre: El iterador fue creado y no se usaron primitivas de la lista que
// la modifiquen desde que se creó.
// Post: Avanza un elemento en la lista y devuelve true. Si no se logró
// (lista vacía o ya había pasado el último elemento) devuelve false.
bool lista_iter_avanzar(lista_iter_t *iter);

// Pre: El iterador fue creado y no se usaron primitivas de la lista que
// la modifiquen desde que se creó.
// Post: Devuelve el elemento en la posición actual del iterador. Devuelve
// NULL si la lista se encuentra vacía o ya pasó el último elemento.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Pre: El iterador fue creado y no se usaron primitivas de la lista que
// la modifiquen desde que se creó.
// Post: Devuelve true si ya se pasó el último elemento de la lista o
// la lista esta vacía, sino false.
bool lista_iter_al_final(const lista_iter_t *iter);

// Pre: El iterador fue creado.
// Post: Destruye el iterador.
void lista_iter_destruir(lista_iter_t *iter);

// Pre: El iterador fue creado y no se usaron primitivas de la lista que
// la modifiquen desde que se creó.
// Post: Inserta un nuevo elemento en la posición actual del iterador.
// El iterador queda posicionado en el elemento insertado. Devuelve
// true si se pudo insertar, false en caso de error.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Pre: El iterador fue creado y no se usaron primitivas de la lista que
// la modifiquen desde que se creó.
// Post: Borra el elemento en la posición actual. El iterador queda
// posicionado en el siguiente elemento. Devuelve el elemento borrado o
// NULL si la lista estaba vacía o ya se pasó el último elemento.
void *lista_iter_borrar(lista_iter_t *iter);

/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/
 
 void pruebas_lista_estudiante(void);

#endif // LISTA_H