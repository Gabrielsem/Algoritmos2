#include "tp0.h"

void swap(int *x, int *y) {
	int aux = *x;
	*x = *y;
	*y = aux;
}


int maximo(int vector[], int n) {
	int max = 0;
	if( n <= 0)
		return -1;

	for(int i = 1; i < n; i++){

		if(vector[i] > vector[max])
			max = i;

	}

	return max;
}


int comparar(int vector1[], int n1, int vector2[], int n2) {

	int i = 0;
	int j = 0;

	while( (i < n1) && (j < n2) ){
		if(vector1[i] == vector2[j]){
			i++;
			j++;
		} else if (vector1[i] < vector2[j]){
			return -1;
		} else {
			return 1;
		}
	}

	if( (i != n1) || (j != n2) ){
		if( i == n1 )
			return -1;

		if( j == n2)
			return 1;
	}

	return 0;
}

void seleccion(int vector[], int n) {
	for(int i = 0; i < n-1; i++){
		
		int max=maximo(vector,n-i);

		swap(vector+n-1-i,vector+max);

	}
}