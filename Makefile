
abb: testing.c testing.h abb.h abb.c pruebas_abb.c
	gcc -g -Wall -Wconversion -Wtype-limits -pedantic -Werror -o pruebas testing.c testing.h abb.h abb.c pruebas_abb.c
	./pruebas

valgrind: pruebas
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./pruebas

.PHONY: abb valgrind