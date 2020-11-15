#include "calc_helper.h"
#include "strutil.h"
#include "pila.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MSG_ERROR "ERROR"

typedef struct calc_token tok_t;

// Lee una línea del standard input, devolviendo un vector a memoria dinamica
// con strings de cada caracter (menos espacios) de la línea por separado. Devuelve
// NULL si falla. Se debe liberar el vector con free_strv() de strutil.h
char** leer_linea(){

	char* linea = NULL;
	size_t tam = 0;
	if(getline(&linea, &tam, stdin) != -1){

		char** vector = infix_split(linea);
		free(linea);
		return vector;
	}
	free(linea);//Documentación de getline pide liberar aunque falle la función
	return NULL;
}

// Recibe el token leído actual y el el último string apilado de un operador anterior.
// Devuelve true si el operador anterior se debe imprimir antes de apilar
// el token actual (según algoritmo de Shunting-yard), sino false.
// Pre: Token actual válido, el string del siguiente operador es en efecto un
// operador parseable por calc_parse() de calc_helper.h
bool debe_escribir(tok_t actual, char* sig_operador){
	
	if(!sig_operador)
		return false;
	tok_t tok_ant;
	calc_parse(sig_operador, &tok_ant);

	return( (tok_ant.type == TOK_OPER) && 
		( (tok_ant.oper.precedencia > actual.oper.precedencia) ||
		( (tok_ant.oper.precedencia == actual.oper.precedencia) &&
		(tok_ant.oper.asociatividad == ASSOC_LEFT) )));
}

// Devuelve true si el string es un token de paréntesis izquierdo, sino false.
// Pre: el string del es un operador parseable por calc_parse() de calc_helper.h
bool es_parentesis_izq(char* operador){

	tok_t tok;
	calc_parse(operador, &tok);
	return tok.type == TOK_LPAREN;
}

// Desapila los primeros elementos de la pila que deban ser impresos en ese
// momento según el token operador actual, y los imprime. Apila el string
// del token actual.
// Pre: El tok_actual debe ser un token de operador válido, y string_actual
// debe poder parsearse a ese token con calc_parse() de calc_helper.h
// Devuelve false en caso de error, sino true.
bool actualizar_operadores(pila_t* operadores, tok_t tok_actual, char* string_actual){

	char* anterior = pila_desapilar(operadores);

	while(debe_escribir(tok_actual, anterior)){
		printf("%s ", anterior);
		anterior = pila_desapilar(operadores);
	}

	return (!anterior || pila_apilar(operadores, (void*) anterior)) && 
		pila_apilar(operadores, (void*) string_actual);
}

// Desapila e imprime hasta encontrar un parentesis izquierdo, y lo descarta.
// Pre: Los strings de la pila deben ser parseables por calc_parse() de
// calc_helper.h. 
void desapilar_parentesis(pila_t* operadores){

	char* anterior = pila_desapilar(operadores);

	while(anterior && !es_parentesis_izq(anterior)){
		printf("%s ", anterior);
		anterior = pila_desapilar(operadores);
	}
}

// Imprime el string pasado en notación posfija.
// Pre: El input es un string de elemtentos en orden de notación infija.
void infix_a_postfix(char** input){

	pila_t* operadores = pila_crear();
	if(!operadores){
		printf("%s\n", MSG_ERROR);
		return;
	}

	while(*input){
		struct calc_token tok;
		if(!calc_parse(*input, &tok)){
			pila_destruir(operadores);
			printf("%s\n", MSG_ERROR);
			return;
		}

		if(tok.type == TOK_NUM){
			printf("%s ", *input);
		} else if (tok.type == TOK_OPER){
			if(!actualizar_operadores(operadores, tok, *input)){
				pila_destruir(operadores);
				printf("%s\n", MSG_ERROR);
				return;
			}
		} else if (tok.type == TOK_LPAREN){
			if(!pila_apilar(operadores, (void*) *input)){
				pila_destruir(operadores);
				printf("%s\n", MSG_ERROR);
				return;
			}
		} else if (tok.type == TOK_RPAREN){
			desapilar_parentesis(operadores);
		}
		input++;
	}

	while(!pila_esta_vacia(operadores)){
		printf("%s ", (char*) pila_desapilar(operadores));
	}
	pila_destruir(operadores);
}

int main(){

	while(!feof(stdin)){
		char** input = leer_linea();
		if(input){
			infix_a_postfix(input);
			printf("\n");
			free_strv(input);
		}
	}

	return 0;
}