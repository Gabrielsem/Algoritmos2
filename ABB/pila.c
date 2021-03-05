#include "pila.h"

#include <stdlib.h>
size_t CANT_VALORES_PARA_AGRANDAR_PILA = 2;//CAPACIDAD / CANT_VALORES_PARA_AGRANDAR_PILA
size_t MULTIPLO_AGRANDAR_PILA = 2;
size_t CANT_VALORES_PARA_ACHICAR_PILA = 4;// analogo a CANT_VALORES_PARA_AGRANDAR_PILA
size_t DIVISOR_ACHICAR_PILA = 2;
size_t CAPACIDAD_INICIAL = 10;
/* Definición del struct pila proporcionado por la cátedra.
 */

struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/
pila_t *pila_crear(void){
	pila_t* pila = malloc(sizeof(pila_t));
	if(pila == NULL){
		return NULL;
	}
	pila->datos = malloc(CAPACIDAD_INICIAL * sizeof(void*));
	if(pila->datos == NULL){
		return NULL;
	}
	pila->cantidad = 0;
	pila->capacidad = CAPACIDAD_INICIAL;
	return pila;
}
void pila_destruir(pila_t *pila){
	free(pila-> datos);
	free(pila);
}
bool pila_esta_vacia(const pila_t *pila){
	return pila->cantidad == 0;
}
void *pila_ver_tope(const pila_t *pila){
	if(pila_esta_vacia(pila)){
		return NULL;
	}
	return pila-> datos[pila->cantidad-1];
}
bool redimensionar(pila_t* pila, size_t tam_nuevo){
	void** datos_nuevo = realloc(pila->datos,tam_nuevo * sizeof(void*));
	if(datos_nuevo == NULL){
		return false;
	}
	pila->datos = datos_nuevo;
	pila->capacidad = tam_nuevo;
	return true;
}
bool pila_apilar(pila_t *pila, void *valor){
	if(pila->cantidad > pila->capacidad /CANT_VALORES_PARA_AGRANDAR_PILA){
		if(!redimensionar(pila,pila->capacidad * MULTIPLO_AGRANDAR_PILA)){
			return false;
		}
	}
	pila->datos[pila->cantidad] = valor;
	pila->cantidad ++;
	return true;
}
void *pila_desapilar(pila_t *pila){
	if(pila_esta_vacia(pila)){
		return NULL;
	}
	void* valor = pila->datos[pila->cantidad-1];
	pila->cantidad --;
	if(pila->cantidad < pila->capacidad /CANT_VALORES_PARA_ACHICAR_PILA){
		redimensionar(pila,pila->capacidad / DIVISOR_ACHICAR_PILA);
	}
	if(pila->cantidad == 0){
		redimensionar(pila,CAPACIDAD_INICIAL);
	}
	return valor;
}
