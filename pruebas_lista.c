#include "lista.h"
#include "testing.h"
#include <stdio.h>

#define CANTIDAD_VOLUMEN 5000 //Cantidad de elementos para pruebas de volumen
#define CANTIDAD_OTROS 10 //Cantidad de elementos para otras pruebas
#define ITERADOR_PARAR 4 //Para pruebas de iterador, en que elemento debería parar. Menor a CANTIDAD_OTROS

//Ejecuta lista_crear(), mostrando mensaje de error si falla
lista_t* crear_lista_print(){
	lista_t *lista = lista_crear();
	print_test("Creando lista para pruebas", lista != NULL);
    if(lista == NULL)
		printf("No se pudo crear lista para pruebas\n");
	return lista;
}

//Ejecuta lista_iter_crear(), mostrando mensaje de error si falla
lista_iter_t* crear_lista_iter_print(lista_t* lista){
	lista_iter_t* iter = lista_iter_crear(lista);
	if(iter == NULL)
		printf("No se pudo crear iterador para pruebas\n");
	return iter;
}

static void pruebas_esta_vacia() {

	//Se crea una lista
	lista_t *lista = crear_lista_print();
    if(lista == NULL){
		return;
	}

 	//Pruebo que una lista nueva esté vacía
	print_test("Lista nueva está vacía", lista_esta_vacia(lista));

	//Pruebo que una lista con elementos no esté vacía
	lista_insertar_primero(lista,(void*) 1);
	print_test("Lista con 1 elemento no está vacía", !lista_esta_vacia(lista));
	lista_insertar_primero(lista,(void*) 2);
	print_test("Lista con 2 elementos no está vacía", !lista_esta_vacia(lista));
	lista_borrar_primero(lista);
	lista_borrar_primero(lista);

	//Pruebo que una lista sin elementos esté vacía
	print_test("Lista sin elementos está vacía", lista_esta_vacia(lista));
    
	lista_destruir(lista, NULL);
}

static void pruebas_insertar_borrar(){

	//Se crea una lista
	lista_t *lista = crear_lista_print();
    if(lista == NULL){
		return;
	}

	//Pruebo que borrar el primer elemento de una lista nueva devuelva NULL
	print_test("Borrar primero en lista nueva devuelve NULL", lista_borrar_primero(lista) == NULL);

	//Pruebo que se agregar y borrar el primero de una lista y sea el mismo elemento
	print_test("Se puede agregar primero a una lista", lista_insertar_primero(lista, (void*) 1));
	print_test("Se borra el primero y es el mismo elemento", lista_borrar_primero(lista) == (void*) 1);

	//Pruebo que se agregar al final de una lista vacía y borrar el primero sea el mismo elemento
	print_test("Se puede agregar último a una lista vacía", lista_insertar_ultimo(lista, (void*) 1));
	print_test("Se borra el primero y es el mismo elemento", lista_borrar_primero(lista) == (void*) 1);

	//Pruebo que se puedan agregar 2 elementos al prinicipio y al borrarlos mantenga el orden
	print_test("Se puede insertar 2 elementos al prinicipio", 
		lista_insertar_primero(lista, (void*) 1) && lista_insertar_primero(lista, (void*) 2) );
	print_test("Borrar primeros 2 elementos en lista devuelve el segundo elemento y luego el primero", 
		(lista_borrar_primero(lista) == (void*) 2) && (lista_borrar_primero(lista) == (void*) 1) );

	//Pruebo que se puedan agregar 2 elementos al final y al borrarlos desde el inicio mantenga el orden
	print_test("Se puede insertar 2 elementos al final de lista vacía", 
		lista_insertar_ultimo(lista, (void*) 1) && lista_insertar_ultimo(lista, (void*) 2) );
	print_test("Borrar primeros 2 elementos devuelve el primer elemento y luego el segundo", 
		(lista_borrar_primero(lista) == (void*) 1) && (lista_borrar_primero(lista) == (void*) 2) );

	//Pruebo que borrar el primer elemento de una lista vacía devuelva NULL
	print_test("Borrar primero en lista vacía devuelve NULL", lista_borrar_primero(lista) == NULL);

	//Pruebo que agregar al principio y borrar NULL sea válido
	print_test("Se puede agregar al principio NULL", lista_insertar_primero(lista,NULL) );
	print_test("Al borrar del principio devuelve NULL", lista_borrar_primero(lista) == NULL);

	//Pruebo que agregar al final y borrar al principio NULL sea válido
	print_test("Se puede agregar al final NULL", lista_insertar_ultimo(lista,NULL) );
	print_test("Al borrar del principio devuelve NULL", lista_borrar_primero(lista) == NULL);

	lista_destruir(lista, NULL);
}

void pruebas_ver(){

	//Se crea una lista
	lista_t *lista = crear_lista_print();
    if(lista == NULL){
		return;
	}

	//Pruebo que ver el primero en una lista nueva devuelva NULL
	print_test("Ver primero en lista nueva da NULL", lista_ver_primero(lista) == NULL);

	//Pruebo que ver el último en una lista nueva devuelva NULL
	print_test("Ver último en lista nueva da NULL", lista_ver_ultimo(lista) == NULL);

	//Pruebo que ver el primero o último en lista con 2 elementos siempre devuelve el correcto
	lista_insertar_primero(lista, (void*) 1);
	print_test("Ver primero en lista con 1 elemento devuelve ese elemento", lista_ver_primero(lista) == (void*) 1);
	print_test("Ver último en lista con 1 elemento devuelve ese elemento", lista_ver_ultimo(lista) == (void*) 1);
	lista_insertar_primero(lista, (void*) 2);
	print_test("Ver primero en lista con 2 elementos devuelve el primero", lista_ver_primero(lista) == (void*) 2);
	print_test("Ver último en lista con 2 elementos devuelve el último", lista_ver_ultimo(lista) == (void*) 1);
	lista_insertar_primero(lista, (void*) 3);
	print_test("Ver primero en lista con 3 elementos devuelve el primero", lista_ver_primero(lista) == (void*) 3);
	print_test("Ver último en lista con 3 elementos devuelve el último", lista_ver_ultimo(lista) == (void*) 1);
	lista_borrar_primero(lista);
	lista_borrar_primero(lista);
	lista_borrar_primero(lista);

	//Pruebo que el primero en una lista vacía devuelva NULL
	print_test("Ver primero en lista vacía da NULL", lista_ver_primero(lista) == NULL);
	//Pruebo que el último en una lista vacía devuelva NULL
	print_test("Ver último en lista vacía da NULL", lista_ver_ultimo(lista) == NULL);

	lista_destruir(lista, NULL);
}

void pruebas_largo(){

	//Se crea una lista
	lista_t *lista = crear_lista_print();
    if(lista == NULL){
		return;
	}

	//Pruebo que ver el largo de una lista nueva de 0
	print_test("Ver largo en lista nueva da 0", lista_largo(lista) == 0);

	//Pruebo que ver el largo de una lista de 1, 2 y 3 elementos,
	//insertando al principio y final, devuelva 1, 2 y 3.
	lista_insertar_primero(lista, (void*) 1);
	print_test("Ver largo en lista con un primer elemento al principio da 1", lista_largo(lista) == 1);
	lista_insertar_primero(lista, (void*) 2);
	print_test("Ver largo en lista con un segundo elemento al principio da 2", lista_largo(lista) == 2);
	lista_insertar_ultimo(lista, (void*) 3);
	print_test("Ver largo en lista con un tercer elemento al final da 3", lista_largo(lista) == 3);
	lista_borrar_primero(lista);
	lista_borrar_primero(lista);
	lista_borrar_primero(lista);

	//Pruebo que ver el largo de una lista vacía de 0
	print_test("Ver largo en lista vacía da 0", lista_largo(lista) == 0);

	lista_destruir(lista, NULL);
}

//Pre: i apunta a un size_t
//Post: Modifica el size_t apuntado (Lo duplica)
void funcion_destruir(void* i){
	*((size_t*) i) *= 2;
}

static void pruebas_destruir(){

	//Se crean dos listas para las pruebas
	lista_t *lista_funcion = crear_lista_print();
	lista_t *lista_null = crear_lista_print();
	if(!lista_null || !lista_funcion){
		return;
	}
	
	//Inserto punteros a elementos de vectores en ambas listas e inicializo vectores
	size_t vector_null[CANTIDAD_OTROS];
	size_t vector_funcion[CANTIDAD_OTROS];
	for(size_t i = 0; i < CANTIDAD_OTROS; i++){
		vector_null[i] = i;
		vector_funcion[i] = i;
		if((!lista_insertar_primero(lista_funcion, vector_funcion+i)) || 
			(!lista_insertar_primero(lista_null, vector_null+i))){
			printf("No se pudo insertar elementos para probar lista_destruir()\n");
			return;
		}
	}

	//Verifico que lista_destruir pasando NULL no modifique elementos
	lista_destruir(lista_null,NULL);
	bool no_modifico = true;
	for(size_t i = 0; i < CANTIDAD_OTROS; i++)
		no_modifico = (vector_null[i] == i);
	print_test("No se modifican elementos al destruir pasando NULL", no_modifico);

	//Verifico que lista_destruir aplique la función que se le pasa a todos los elementos
	lista_destruir(lista_funcion,funcion_destruir);
	bool aplico_funcion = true;
	size_t elemento;
	for(size_t i = 0; i < CANTIDAD_OTROS; i++){
		elemento = i;
		funcion_destruir(&elemento);
		aplico_funcion = (vector_funcion[i] == elemento);
	}
	print_test("Se aplica la función pasada al destruir lista", aplico_funcion);

}

//Pruebas con CANTIDAD_VOLUMEN elementos
static void pruebas_volumen(){

	//Se crea una lista
	lista_t *lista = crear_lista_print();
    if(lista == NULL){
		return;
	}

	bool pudo_insertar = true;
	bool mantuvo_orden = false;

	//Inserto CANTIDAD_VOLUMEN elementos al final
	for(size_t i = 0; i < CANTIDAD_VOLUMEN; i++){
		if( !lista_insertar_ultimo(lista, (void*) i) )
			pudo_insertar = false;
	}

	//Borro los elementos desde el principio y verifico que sea en el orden correcto
	if(pudo_insertar){
		mantuvo_orden = true;

		for(size_t i = 0; i < CANTIDAD_VOLUMEN; i++){
			mantuvo_orden = (lista_borrar_primero(lista) == (void*) i);
		}
	}

	print_test("Prueba de volumen: Insertar elementos", pudo_insertar);
	print_test("Prueba de volumen: Mantuvo orden al borrar los elementos", mantuvo_orden);
	print_test("Prueba de volumen: Devuelve NULL después de borrar todos los elementos", lista_borrar_primero(lista) == NULL);

	lista_destruir(lista, NULL);
}

//Pre: i apunta a un size_t
//Post: Modifica el size_t apuntado (Lo duplica)
bool funcion_iterar_dup(void* i, void* extra){
	*((size_t*) i) *= 2;
	return true;
}

//Pre: i y extra apunta a un size_t
//Post: Le suma a lo apuntado por extra el valor de lo apuntado por i.
//Devuelve falso si lo apuntado por i es mayor a ITERADOR_PARAR, y no hace la suma.
bool funcion_iterar_sum(void* i, void* extra){
	if(*((size_t*) i) > ITERADOR_PARAR)
		return false;
	*((size_t*) extra) += *((size_t*) i);
	return true;
}

static void pruebas_iterar(){

	//Se crea una lista
	lista_t *lista = crear_lista_print();
    if(lista == NULL){
		return;
	}
	
	//Inserto punteros a elementos de vector en la lista en el mismo orden e inicializo vector
	size_t vector[CANTIDAD_OTROS];
	for(size_t i = 0; i < CANTIDAD_OTROS; i++){
		vector[i] = i;
		if(!lista_insertar_ultimo(lista, vector+i)){
			printf("No se pudo insertar elementos para probar lista_iterar()\n");
			return;
		}
	}

	//Verifico que lista_iterar pasando función NULL no modifique elementos
	lista_iterar(lista,NULL,NULL);
	bool no_modifico = true;
	for(size_t i = 0; i < CANTIDAD_OTROS; i++){
		size_t* dato = (size_t*) lista_borrar_primero(lista);
		no_modifico = (dato == vector+i) && (vector[i] = i);
		if(!lista_insertar_ultimo(lista, dato)){
			printf("No se pudo insertar elementos para probar lista_iterar()\n");
			return;
		}
	}
	print_test("No se modifican elementos al iterar pasando NULL", no_modifico);

	//Verifico que lista_iterar pueda cortar la iteración antes de tiempo y modificar el extra
	size_t suma = 0, suma_manual = 0;
	lista_iterar(lista, funcion_iterar_sum, &suma);
	for(size_t i = 0; i <= ITERADOR_PARAR; i++){
		suma_manual += vector[i];
	}
	print_test("Se modifica acordemente el extra y corta a tiempo al iterar la lista", suma == suma_manual);

	//Verifico que lista_iterar aplique la función que se le pasa a todos los elementos
	lista_iterar(lista, funcion_iterar_dup, NULL);
	bool aplico_funcion = true;
	size_t elemento;
	for(size_t i = 0; i < CANTIDAD_OTROS; i++){
		elemento = i;
		funcion_iterar_dup(&elemento, NULL);
		aplico_funcion = (vector[i] == elemento);
	}
	print_test("Se aplica la función pasada al iterar la lista", aplico_funcion);

	lista_destruir(lista, NULL);
}

static void pruebas_iter_avanzar_actual(){

	//Se crea una lista
	lista_t *lista = crear_lista_print();
    if(lista == NULL){
		return;
	}

	//Se crea un iterador
	lista_iter_t* iter = crear_lista_iter_print(lista);
	if(iter == NULL){
		return;
	}

	//Verifico que avanzar en lista nueva devuelva false
	print_test("Avanzar en lista nueva devuelve false", !lista_iter_avanzar(iter));

	//Verifico que ver actual en lista nueva devuelva NULL
	print_test("Ver actual en lista nueva devuelve NULL", !lista_iter_ver_actual(iter));
	
	//Inserto elementos en la lista
	for(size_t i = 0; i < CANTIDAD_OTROS; i++){
		if(!lista_insertar_ultimo(lista, (void*) i)){
			printf("No se pudo insertar elementos para probar lista_iter_avanzar() y lista_iter_ver_actual()\n");
			return;
		}
	}

	lista_iter_destruir(iter);
	iter = lista_iter_crear(lista);
	if(iter == NULL){
		printf("No se pudo crear iterador para probar lista_iter_avanzar() y lista_iter_ver_actual()\n");
		return;
	}

	//Verifico que al ir avanzando vaya viendo el elemento correcto
	bool avanza_correcto = true;
	for(size_t i = 0; i < CANTIDAD_OTROS; i++)
		avanza_correcto = ((lista_iter_ver_actual(iter) == (void*) i) && lista_iter_avanzar(iter));
	print_test("Al avanzar el elemento actual va siendo el correcto y puede avanzar", avanza_correcto);

	//Verifico que avanzar al final devuelva false
	print_test("Avanzar al final de lista devuelve false", !lista_iter_avanzar(iter));

	//Verifico que ver actual al final de la lista devuelva NULL
	print_test("Ver actual al final de la lista devuelve NULL", !lista_iter_ver_actual(iter));

	lista_iter_destruir(iter);
	lista_destruir(lista, NULL);
}

static void pruebas_iter_final(){

	//Se crea una lista
	lista_t *lista = crear_lista_print();
    if(lista == NULL){
		return;
	}

	//Se crea un iterador
	lista_iter_t* iter = crear_lista_iter_print(lista);
	if(iter == NULL){
		return;
	}

	//Verifico que este al final en lista vacia
	print_test("Iterador esta al final en lista vacía", lista_iter_al_final(iter));
	lista_iter_destruir(iter);

	//Inserto elemento a lista y creo un nuevo iterador
	lista_insertar_primero(lista, (void*) 1);
	iter = crear_lista_iter_print(lista);
	if(iter == NULL){
		return;
	}

	//Verifico que no este al final de la lista en iterador al inicio
	print_test("Iterador al inicio de lista con elementos no esta al final", !lista_iter_al_final(iter));

	//Verifico que al avanzar, efectivamente este al final
	lista_iter_avanzar(iter);
	print_test("Iterador al final de lista efectivamente esta al final", lista_iter_al_final(iter));

	lista_iter_destruir(iter);
	lista_destruir(lista, NULL);
}

static void pruebas_iter_insertar_borrar(){

	//Se crea una lista
	lista_t *lista = crear_lista_print();
    if(lista == NULL){
		return;
	}
	//Se crea un iterador
	lista_iter_t* iter = crear_lista_iter_print(lista);
	if(iter == NULL){
		return;
	}

	//Verifico que borrar con iterador en lista vacía de NULL
	print_test("Borrar con iterador en lista vacía da NULL", !lista_iter_borrar(iter));
	//Inserto dos elementos en lista vacía
	print_test("Iterador en lista vacía inserta elemento", lista_iter_insertar(iter, (void*) 1));
	print_test("Iterador puede insertar segundo elemento", lista_iter_insertar(iter, (void*) 4));
	//Verifico que borrar con iterador al final de NULL
	print_test("Borrar con iterador al final da NULL", !lista_iter_borrar(iter));

	lista_iter_destruir(iter);
	//Se crea un iterador
	iter = crear_lista_iter_print(lista);
	if(iter == NULL){
		return;
	}

	//Tras avanzar, inserto 2 nuevos elementos en el medio
	print_test("Con nuevo iterador, avanzar e insertar tercer y cuarto elemento en el medio",
		lista_iter_avanzar(iter) && lista_iter_insertar(iter, (void*) 2) && lista_iter_insertar(iter, (void*) 3));
	//Verifico que las primera y ultima posiciones de la lista sean correctas	
	print_test("Primero de lista es correcto", lista_ver_primero(lista) == (void*) 1);
	print_test("Ultimo de lista es correcto", lista_ver_ultimo(lista) == (void*) 4);

	lista_iter_destruir(iter);
	//Se crea un iterador
	iter = crear_lista_iter_print(lista);
	if(iter == NULL){
		return;
	}

	//Verifico poder borrar elemento del medio y último correctamente, actualizando el último de la lista
	print_test("Con nuevo iterador, avanzar y borrar segundo elemento", 
		lista_iter_avanzar(iter) && (lista_iter_borrar(iter) == (void*) 2));
	print_test("Avanzar y borrar último elemento", 
		lista_iter_avanzar(iter) && (lista_iter_borrar(iter) == (void*) 4));
	print_test("Nuevo último de lista es correcto", lista_ver_ultimo(lista) == (void*) 3);

	lista_iter_destruir(iter);
	//Se crea un iterador
	iter = crear_lista_iter_print(lista);
	if(iter == NULL){
		return;
	}

	//Verifico que al borrar los 2 elementos restantes quede en orden correcto, y se acutalice el primer elemento
	print_test("Con nuevo iterador, borrar el primer elemento", lista_iter_borrar(iter) == (void*) 1);
	print_test("Nuevo primero de lista es correcto", lista_ver_primero(lista) == (void*) 3);
	print_test("Borrar ultimo elemento", lista_iter_borrar(iter) == (void*) 3);


	lista_iter_destruir(iter);
	lista_destruir(lista, NULL);
}

void pruebas_lista_estudiante() {
	printf("\n-PRUEBAS lista_esta_vacia()-\n");
	pruebas_esta_vacia();
	printf("\n-PRUEBAS lista_insertar_primero(), lista_borrar_primero() y lista_insertar_ultimo-\n");
	pruebas_insertar_borrar();
	printf("\n-PRUEBAS lista_ver_primero() y lista_ver_ultimo()-\n");
	pruebas_ver();
	printf("\n-PRUEBAS lista_largo()-\n");
	pruebas_largo();
	printf("\n-PRUEBAS lista_destruir()-\n");
	pruebas_destruir();
	printf("\n-PRUEBAS DE VOLUMEN-\n");
	pruebas_volumen();
	printf("\n-PRUEBAS lista_iterar() (iterador interno)-\n");
	pruebas_iterar();
	printf("\n-PRUEBAS lista_iter_avanzar() y lista_iter_ver_actual() (iterador externo)-\n");
	pruebas_iter_avanzar_actual();
	printf("\n-PRUEBAS lista_iter_al_final() (iterador externo)-\n");
	pruebas_iter_final();
	printf("\n-PRUEBAS lista_iter_insertar() y lista_iter_borrar (iterador externo)-\n");
	pruebas_iter_insertar_borrar();
	return;
}

#ifndef CORRECTOR 

int main(void) {
    pruebas_lista_estudiante();
    return failure_count() > 0; 
}

#endif
