#include "strutil.h"
#include "testing.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void pruebas_substr(){

	const char* string = "Hola mundo";

	char* prueba = substr(string, 6);
	print_test("Caso normal", strcmp(prueba, "Hola m") == 0);
	free(prueba);

	prueba = substr(string + 2, 6);
	print_test("Sumando a puntero", strcmp(prueba, "la mun") == 0);
	free(prueba);

	prueba = substr(string, 25);
	print_test("Excediendo largo del string", strcmp(prueba, string) == 0);
	free(prueba);
}

void pruebas_split(){

	const char* string_1 = "ab,c,de";
	const char* string_2 = "ab,,de";
	const char* string_3 = ",asd,";
	const char* string_4 = "asd";
	const char* string_5 = "";
	const char* string_6 = ",";

	char** prueba = split(string_1, ',');
	print_test("Caso normal", !strcmp(prueba[0], "ab") && !strcmp(prueba[1], "c") &&
		!strcmp(prueba[2], "de") && !prueba[3] );
	free_strv(prueba);

	prueba = split(string_2, ',');
	print_test("Doble coma", !strcmp(prueba[0], "ab") && !strcmp(prueba[1], "") &&
		!strcmp(prueba[2], "de") && !prueba[3] );
	free_strv(prueba);

	prueba = split(string_3, ',');
	print_test("Coma al inicio y al final", !strcmp(prueba[0], "") && !strcmp(prueba[1], "asd") &&
		!strcmp(prueba[2], "") && !prueba[3] );
	free_strv(prueba);

	prueba = split(string_4, ',');
	print_test("No está el separador", !strcmp(prueba[0], "asd") && !prueba[1] );
	free_strv(prueba);

	prueba = split(string_4, ',');
	print_test("Pasar \\0 como separador", !strcmp(prueba[0], "asd") && !prueba[1] );
	free_strv(prueba);

	prueba = split(string_5, ',');
	print_test("String vacío", !strcmp(prueba[0], "") && !prueba[1] );
	free_strv(prueba);

	prueba = split(string_6, ',');
	print_test("String de sólo el separador", !strcmp(prueba[0], "") && !strcmp(prueba[1], "") && 
		!prueba[2] );
	free_strv(prueba);

}

void pruebas_join(){

	char** vector_1 = split("abc,def,ghi", ',');
	char* vector_2[1] = {NULL};
	char* vector_3[2] = {"", NULL};
	char* vector_4[3] = {"", "", NULL};
	char* vector_5[2] = {"abc", NULL};

	char* string = join(vector_1, ';');
	print_test("Caso normal", !strcmp(string, "abc;def;ghi"));
	free(string);

	string = join(vector_2, ';');
	print_test("Vector vacío", !strcmp(string, ""));
	free(string);

	string = join(vector_3, ';');
	print_test("Vector con string vacío", !strcmp(string, ""));
	free(string);

	string = join(vector_4, ';');
	print_test("Vector con 2 strings vacíos", !strcmp(string, ";"));
	free(string);

	string = join(vector_5, ';');
	print_test("Vector con un solo string", !strcmp(string, "abc"));
	free(string);

	string = join(vector_1, '\0');
	print_test("Unir con '\\0'", !strcmp(string, "abcdefghi"));
	free(string);

	free_strv(vector_1);
}

void pruebas_tp1() {
	printf("\n---------PRUEBAS substr()---------\n");
	pruebas_substr();
	printf("\n---------PRUEBAS split()---------\n");
	pruebas_split();
	printf("\n---------PRUEBAS join()---------\n");
	pruebas_join();
}

int main(void) {
    pruebas_tp1();
    return failure_count() > 0; 
}

