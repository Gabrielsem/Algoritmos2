#ifndef ABB_H
#define ABB_H

#include <stdbool.h>
#include <stddef.h>


/* ******************************************************************
 *                  		PRIMITIVAS ABB
 * *****************************************************************/

typedef struct abb abb_t;

// tipo de función para comparar claves (define el orden del abb)
typedef int (*abb_comparar_clave_t) (const char *, const char *);
// tipo de función para destruir dato
typedef void (*abb_destruir_dato_t) (void *);
// tipo de función para visitar en iterador in-order
typedef bool (*abb_visitar_t)(const char *, void *, void *);
typedef void (*abb_visitar_rng_t)(const char *, void *, void *);

/* Crea el abb
 */
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

/* Guarda un elemento en el abb, si la clave ya se encuentra en la
 * estructura, la reemplaza. De no poder guardarlo devuelve false.
 * Pre: La estructura abb fue inicializada
 * Post: Se almacenó el par (clave, dato)
 */
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

/* Borra un elemento del abb y devuelve el dato asociado.  Devuelve
 * NULL si el dato no estaba.
 * Pre: La estructura abb fue inicializada
 * Post: El elemento fue borrado de la estructura y se lo devolvió,
 * en el caso de que estuviera guardado.
 */
void *abb_borrar(abb_t *arbol, const char *clave);

/* Obtiene el valor de un elemento del abb, si la clave no se encuentra
 * devuelve NULL.
 * Pre: La estructura abb fue inicializada
 */
void *abb_obtener(const abb_t *arbol, const char *clave);

/* Determina si clave pertenece o no al abb.
 * Pre: La estructura abb fue inicializada
 */
bool abb_pertenece(const abb_t *arbol, const char *clave);

/* Devuelve la cantidad de elementos del abb.
 * Pre: La estructura abb fue inicializada
 */
size_t abb_cantidad(abb_t *arbol);

/* Destruye la estructura liberando la memoria pedida y llamando a la función
 * destruir para cada par (clave, dato).
 * Pre: La estructura abb fue inicializada
 * Post: La estructura abb fue destruida
 */
void abb_destruir(abb_t *arbol);



/* ******************************************************************
 *					PRIMITIVAS ITERADOR INTERNO
 * *****************************************************************/

/* Recorre los elementos del ABB in-order, aplicándoles la función
 * visitar. Si esta devuelve falso, se deja de recorrer el ABB.
 */
void abb_in_order(abb_t *arbol, abb_visitar_t visitar, void *extra);

/* Recorre los elementos del ABB in-order en un rango, aplicándoles la función
 * visitar. Comienza a recorrer a partir de la clave ini hasta la clave fin.
 * Si ini es NULL, recorre desde el inicio.
 * Si fin es NULL, recorre hasta el final.
 */
void abb_in_order_rng(abb_t *arbol, abb_visitar_rng_t visitar, void *extra, const char* ini, const char* fin);


/* ******************************************************************
 *					PRIMITIVAS ITERADOR EXTERNO
 * *****************************************************************/

typedef struct abb_iter abb_iter_t;

// Crea iterador
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

// Avanza iterador
bool abb_iter_in_avanzar(abb_iter_t *iter);

// Devuelve clave actual, esa clave no se puede modificar ni liberar.
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

// Comprueba si terminó la iteración
bool abb_iter_in_al_final(const abb_iter_t *iter);

// Destruye iterador
void abb_iter_in_destruir(abb_iter_t* iter);

#endif  // ABB_H
