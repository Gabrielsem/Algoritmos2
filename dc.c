#define _POSIX_C_SOURCE 200809L
#include "calc_helper.h"
#include "strutil.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

#define MSG_ERROR "ERROR"

// FUNCIONES PARA CADA OPERACION
// Cada operación utiliza los operandos del vector numeros y se guarda el resultado en la variable.
// Leen el arreglo de numeros en orden inverso: El último operando, el de la derecha, será el de la
// posición 0 del arreglo (para aquellas operaciones que influya el orden de los operandos).
// Pre: El vector de números tiene suficientes elementos para esa operación
// Devuelve true si se pudo realizar la operacion, false en caso de error.
typedef bool (*operacion_f)(calc_num* numeros, calc_num* resultado);

bool suma(calc_num* numeros, calc_num* resultado){
	*resultado = numeros[0] + numeros[1];
	return true;
}

bool resta(calc_num* numeros, calc_num* resultado){
	*resultado = numeros[1] - numeros[0];
	return true;
}

bool multiplicacion(calc_num* numeros, calc_num* resultado){
	*resultado = numeros[0] * numeros[1];
	return true;
}

bool division(calc_num* numeros, calc_num* resultado){

	if(numeros[0] == 0)
		return false;

	*resultado = numeros[1]/numeros[0];
	return true;
}

bool potencia(calc_num* numeros, calc_num* resultado){

	double valor = pow((double) numeros[1], (double) numeros[0]);
	if(!isfinite(valor))
		return false;

	*resultado = (calc_num) valor;
	return true;
}

bool logaritmo(calc_num* numeros, calc_num* resultado){

	double valor_0 = log( (double) numeros[0]);
	double valor_1 = log( (double) numeros[1]);
	if(!isfinite(valor_0) || !isfinite(valor_1) || (valor_0 == 0)){
		return false;
	}
	
	*resultado = (calc_num) (valor_1/valor_0);
	return true;
}

bool raiz(calc_num* numeros, calc_num* resultado){

	double valor = sqrt( (double) numeros[0]);
	if(!isfinite(valor))
		return false;
	
	*resultado = (calc_num) valor;
	return true;
}

bool ternario(calc_num* numeros, calc_num* resultado){

	*resultado = numeros[2] ? numeros[1] : numeros[0];
	return true;
}

// Lee una línea del standard input, devolviendo un vector a memoria dinamica
// con strings de cada palabra de la línea por separado. Devuelve NULL si falla.
// Se debe liberar el vector con free_strv() de strutil.h
char** leer_linea(){

	char* linea = NULL;
	size_t tam = 0;
	if(getline(&linea, &tam, stdin) != -1){

		char** vector = dc_split(linea);
		free(linea);
		return vector;
	}
	free(linea); //Documentación de getline pide liberar aunque falle la función
	return NULL;
}

// Recibe la operación en un enum oper_type de calc_helper.h
// Devuelve un puntero a una función que realiza la operación correspondiente.
operacion_f operacion(enum oper_type op){
	
	switch(op){
		case OP_ADD:
			return suma;
		case OP_SUB:
			return resta;
		case OP_MUL:
			return multiplicacion;
		case OP_DIV:
			return division;
		case OP_POW:
			return potencia;
		case OP_LOG:
			return logaritmo;
		case OP_RAIZ:
			return raiz;
		default: //OP_TERN
			return ternario;
	}
}

// Recibe una pila de números y un operador.
// Se realiza la operación correspondiente con los números en el tope de la pila,
// y se vuelve a apilar el resultado.
// Pre: oper válido, pila ya creada.
// Devuelve false en caso de error, pudiendo haber modificado la pila; sino true.
bool operar(pilanum_t* pila, calc_operador oper){

	calc_num numeros[oper.num_operandos];
	for(int i = 0; i < oper.num_operandos; i++){
		if(!desapilar_num(pila, numeros+i))
			return false;
	}
	operacion_f funcion = operacion(oper.op);
	calc_num resultado;
	if(!funcion(numeros, &resultado))
		return false;

	apilar_num(pila, resultado);
	return true;
}

// Recibe un arreglo con números y operaciones, y realiza dichas operaciones considerando
// notación posfija, cargando el resultado del calculo.
// Pre: el arreglo input ya tiene cargados los números y operaciones.
// Devuelve verdadero si se pudo realizar la operación, falso en caso de error.
bool calculo(char** input, calc_num* resultado){

	pilanum_t* pila = pilanum_crear();
	if(!pila)
		return false;

	while(*input){
		struct calc_token tok;
		if(!calc_parse(*input, &tok)){
			pilanum_destruir(pila);
			return false;
		}

		if(tok.type == TOK_NUM){
			apilar_num(pila, tok.value);
		} else if(!operar(pila, tok.oper)){
			pilanum_destruir(pila);
			return false;
		}

		input++;
	}

	if(!desapilar_num(pila, resultado) || desapilar_num(pila, resultado)){
		pilanum_destruir(pila);
		return false;
	}

	pilanum_destruir(pila);
	return true;
}

int main(){

	double hola = log(-2);
	printf("???: %i\n%f\n\n", hola == NAN, hola);

	while(!feof(stdin)){
		char** input = leer_linea();
		calc_num resultado;
		if(input){
			if(calculo(input, &resultado)){
				printf("%li\n", resultado);
			} else {
				printf("%s\n", MSG_ERROR);
			}
			free_strv(input);
		}
	}

	return 0;
}