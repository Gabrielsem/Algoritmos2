
str: testing.c testing.h pruebas_str.c strutil.h strutil.c
	gcc -g -Wall -Wconversion -Wtype-limits -pedantic -Werror -o pruebas testing.c testing.h pruebas_str.c strutil.h strutil.c
	./pruebas

dc: calc_helper.c calc_helper.h strutil.h strutil.c dc.c pila.c pila.h
	gcc -g -Wall -Wconversion -Wtype-limits -pedantic -Werror -o dc calc_helper.c calc_helper.h strutil.h strutil.c dc.c pila.c pila.h -lm
	cat cuentas.txt
	./dc < cuentas.txt

infix: calc_helper.c calc_helper.h strutil.h strutil.c infix.c pila.c pila.h
	gcc -g -Wall -Wconversion -Wtype-limits -pedantic -Werror -o infix calc_helper.c calc_helper.h strutil.h strutil.c infix.c pila.c pila.h -lm
	cat cuentas_infix.txt
	./infix < cuentas_infix.txt

valgrind_str: pruebas
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./pruebas

valgrind_dc: dc
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./dc < cuentas.txt

valgrind_infix: infix
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./infix < cuentas_infix.txt

.PHONY: str dc infix