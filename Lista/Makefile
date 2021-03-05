
lista: testing.c testing.h lista.c lista.h pruebas_lista.c
	gcc -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -o pruebas *.c
	./pruebas

valgrind: pruebas
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./pruebas
