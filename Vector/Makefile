
tp1: testing.c testing.h vector.c vector.h pruebas_vector.c
	gcc -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -o pruebas *.c

valgrind: pruebas
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./pruebas
