
heap: testing.c testing.h main.c heap.h heap_pruebas.c
	gcc -g -Wall -Wconversion -Wtype-limits -pedantic -Werror -o pruebas testing.c testing.h main.c heap.h heap_pruebas.c heap.c
	./pruebas

valgrind: pruebas
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./pruebas

.PHONY: heap valgrind