
hash: testing.c testing.h main.c hash.h hash_pruebas.c
	gcc -g -Wall -Wconversion -Wtype-limits -pedantic -Werror -o pruebas testing.c testing.h main.c hash.h hash_pruebas.c
	./pruebas

valgrind: pruebas
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./pruebas

.PHONY: hash valgrind