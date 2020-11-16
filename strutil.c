#define _POSIX_C_SOURCE 200809L
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

//----AUXILIARES----

// Devuelve la cantidad de apariciones del caracter en el string.
// Devuelve 0 si el caracter es '\0'.
size_t apariciones(const char* str, char caracter){

	if(caracter == '\0')
		return 0;

	size_t contador = 0;
	char* sig_aparicion = strchr(str, caracter);
	while(sig_aparicion){
	    contador++;
	    sig_aparicion = strchr(sig_aparicion+1, caracter);
	}
	return contador;
}

//Devuelve el largo total de la union de las cadenas del arreglo de cadenas,
//intercalandolas con ese separador. No incluye el \0.
//Si el separador es '\0', se considera que no hay separador.
size_t largo_join(char** strv, char sep){

	char* actual = strv[0];
	size_t largo = 0;
	size_t cantidad = 0;

	while(actual){
	    largo += strlen(actual);
	    cantidad++;
	    actual = strv[cantidad];
	}

	if(sep && cantidad)
		largo += cantidad-1;

	return largo;
}

//----PRIMITIVAS----

void free_strv(char *strv[]){

	for(size_t i = 0; strv[i]; i++){
		free(strv[i]);
	}
	free(strv);
}

char *substr(const char *str, size_t n){
    return strndup(str, n);
}


char **split(const char *str, char sep){

    size_t cantidad_strings = apariciones(str, sep) + 1;
    char** vector = malloc((cantidad_strings+1)*sizeof(char*));
    if(!vector)
        return NULL;

    vector[cantidad_strings] = NULL;

    const char* actual = str;
    const char* sig_aparicion = strchr(actual, sep);
    for(size_t i = 0; i < cantidad_strings-1; i++){

    	vector[i] = strndup(actual, (size_t) (sig_aparicion-actual));
        if(!vector[i]){
        	free_strv(vector);
            return NULL;
        }

        actual = sig_aparicion+1;
        sig_aparicion = strchr(actual, sep);
    }

    vector[cantidad_strings-1] = strdup(actual);
	if(!vector[cantidad_strings-1]){
       	free_strv(vector);
		return NULL;
    }

    return vector;
}

char *join(char **strv, char sep){

    size_t largo = largo_join(strv, sep);

    char* string = malloc(sizeof(char)*(largo+1));
    if(!string){
        return NULL;
    }

    char* pos_actual = stpcpy(string, strv[0] ? strv[0] : "");
    size_t i = 1;
    while(pos_actual-string < largo){

    	if(sep){
    	    *pos_actual = sep;
    	    pos_actual++;
    	}
    	pos_actual = stpcpy(pos_actual, strv[i]);
    	i++;
    }

    return string;
}
