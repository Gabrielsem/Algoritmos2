
str: testing.c testing.h pruebas_str.c strutil.h strutil.c
	gcc -g -Wall -Wconversion -Wtype-limits -pedantic -Werror -o pruebas testing.c testing.h pruebas_str.c strutil.h strutil.c
	./pruebas

dc: calc_helper.c calc_helper.h strutil.h strutil.c dc.c pila.c pila.h
	gcc -g -Wall -Wconversion -Wtype-limits -pedantic -Werror -o dc calc_helper.c calc_helper.h strutil.h strutil.c dc.c pila.c pila.h -lm
	./dc

valgrind_str: pruebas
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./pruebas

valgrind_dc: dc
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./dc
