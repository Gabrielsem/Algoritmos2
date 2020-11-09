
tp1: testing.c testing.h calc_helper.c calc_helper.h pruebas_tp1.c strutil.h strutil.c dc.c infix.c
	gcc -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -o pruebas *.c
	./pruebas

valgrind: pruebas
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./pruebas
