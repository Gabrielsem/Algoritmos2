#include "pila.h"
#include "testing.h"
#include <stdio.h>
#include <stdbool.h>

#define CANTIDAD_VOLUMEN 5000
#define NEGRO "\x1b[40m"
#define RESET_COLOR "\x1b[0m"

static void prueba_pila_vacia() {

	//Se crea una pila
    pila_t *pila = pila_crear();
    print_test("Creando pila para probar pila_esta_vacia()", pila != NULL);
    if(pila == NULL){
    	printf("No se pudo crear pila para probar pila_esta_vacia()\n");
    	return;
    }

    //Pruebo que una pila nueva esté vacía
    print_test("Pila nueva está vacía", pila_esta_vacia(pila));

    //Pruebo que una pila con elementos no esté vacía
	pila_apilar(pila,(void*) 1);
	print_test("Pila con 1 elemento no está vacía", !pila_esta_vacia(pila));
	pila_apilar(pila,(void*) 2);
	print_test("Pila con 2 elementos no está vacía", !pila_esta_vacia(pila));
	pila_desapilar(pila);
	pila_desapilar(pila);

	//Pruebo que una pila sin elementos esté vacía
	print_test("Pila sin elementos está vacía", pila_esta_vacia(pila));
    
    pila_destruir(pila);
}

//Pruebas con CANTIDAD_VOLUMEN elementos
static void pruebas_volumen(pila_t* pila){

	bool pudo_apilar = true;
	bool mantuvo_orden = false;

	//Apilo CANTIDAD_VOLUMEN elementos
	for(size_t i = 0; i < CANTIDAD_VOLUMEN; i++){
		if( !pila_apilar(pila, (void*) i) )
			pudo_apilar = false;
	}

	//Desapilo los elementos y verifico que sea en el orden correcto
	if(pudo_apilar){
		mantuvo_orden = true;

		for(size_t i = 0; i < CANTIDAD_VOLUMEN; i++){
			mantuvo_orden = (pila_desapilar(pila) == (void*) (CANTIDAD_VOLUMEN-1-i));
		}
	} 

	print_test("Prueba de volumen: Apilar elementos", pudo_apilar);
	print_test("Prueba de volumen: Mantuvo orden al desapilar los elementos", mantuvo_orden);
	print_test("Prueba de volumen: Devuelve NULL después de desapilar todos los elementos", pila_desapilar(pila) == NULL);
}

//Pruebas para las funciones pila_apilar y pila_desapilar
static void prueba_pila_apilar_desapilar(){
	//Se crea una pila
	pila_t *pila = pila_crear();
	print_test("Creando pila para probar pila_apilar() y pila_desapilar()", pila != NULL);
	if(pila == NULL){
		printf("No se pudo crear pila para probar pila_apilar() y pila_desapilar()\n");
		return;
	}

	//Pruebo que desapilar una pila nueva devuelva NULL
	print_test("Desapilar pila nueva devuelve NULL", pila_desapilar(pila) == NULL);

	//Pruebo que se pueda apilar y desapilar el mismo elemento
	print_test("Se puede apilar un elemento", pila_apilar(pila,(void*) 1));
	print_test("Se desapila el mismo elemento", pila_desapilar(pila) == (void*) 1);

	//Pruebo que se puedan apilar 2 elementos y al desapilar mantenga el orden
	print_test("Se puede apilar 2 elementos", pila_apilar(pila,(void*) 1) && pila_apilar(pila,(void*) 2) );
	print_test("Desapilar en pila de 2 elementos devuelve el segundo elemento y luego el primero", 
		(pila_desapilar(pila) == (void*) 2) && (pila_desapilar(pila) == (void*) 1) );

	//Pruebo que desapilar una pila vacía devuelva NULL
	print_test("Desapilar pila vacía devuelve NULL", pila_desapilar(pila) == NULL);

	pruebas_volumen(pila);
	
	pila_destruir(pila);
}

static void pruebas_pila_null(){
	//Se crea una pila
	pila_t *pila = pila_crear();
	print_test("Creando pila para probar pila_ver_tope()", pila != NULL);
	if(pila == NULL){
		printf("No se pudo crear pila para probar pila_ver_tope()\n");
		return;
	}

	//Pruebo que ver el tope en una pila nueva devuelva NULL
	print_test("Ver tope en pila nueva da NULL", pila_ver_tope(pila) == NULL);

	//Pruebo que ver el tope en una pila siempre devuelva el último elemento apilado
	pila_apilar(pila,(void*) 1);
	print_test("Ver tope en pila con 1 elemento devuelve ese elemento", pila_ver_tope(pila) == (void*) 1);
	pila_apilar(pila,(void*) 2);
	print_test("Ver tope en pila con 2 elementos devuelve el 2do elemento", pila_ver_tope(pila) == (void*) 2);
	pila_desapilar(pila);
	pila_desapilar(pila);

	//Pruebo que  el tope en una pila vacía devuelva NULL
	print_test("Ver tope en pila vacía da NULL", pila_ver_tope(pila) == NULL);

	pila_destruir(pila);
}

static void pruebas_pila_ver_tope(){
	//Se crea una pila
	pila_t *pila = pila_crear();
	print_test("Creando pila para pruebas apilando NULL", pila != NULL);
	if(pila == NULL){
		printf("No se pudo crear pila para pruebas apilando NULL\n");
		return;
	}

	//Pruebo que apilar NULL sea válido
	print_test("Se puede apilar NULL", pila_apilar(pila,NULL));

	//Pruebo que pila con NULL apilado no este vacía.
	print_test("Pila con NULL apilado no esta vacía", !pila_esta_vacia(pila));

	//Pruebo ver tope con pila con NULL.
	print_test("Pila con NULL apilado tiene tope correcto", !pila_ver_tope(pila));

	//Pruebo que desapilar NULL sea válido
	print_test("Se puede desapilar NULL", pila_desapilar(pila) == NULL);

	pila_destruir(pila);
}

void pruebas_pila_estudiante() {
	printf(NEGRO"========== PRUEBAS pila_esta_vacia() =========="RESET_COLOR"\n");
    prueba_pila_vacia();
	printf(NEGRO"=== PRUEBAS pila_apilar() y pila_desapilar() ==="RESET_COLOR"\n");
    prueba_pila_apilar_desapilar();
	printf(NEGRO"=========== PRUEBAS pila_ver_tope() ==========="RESET_COLOR"\n");
    pruebas_pila_null();
   	printf(NEGRO"=== PRUEBAS apilando NULL ==="RESET_COLOR"\n");
    pruebas_pila_ver_tope();
    printf(NEGRO"ERRORES TOTALES: %i"RESET_COLOR"\n",failure_count());
}


#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_pila_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
