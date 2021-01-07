
zyxcba: cola.c cola.h heap.c heap.h hash.c hash.h pacutil.c pacutil.h lista.c lista.h csv.c csv.h funciones_tp2.h funciones_tp2.c mensajes.h strutil.c strutil.h zyxcba.c
	gcc -g -Wall -Wconversion -Wtype-limits -pedantic -Werror -o zyxcba cola.c cola.h heap.c heap.h hash.c hash.h pacutil.c pacutil.h lista.c lista.h csv.c csv.h funciones_tp2.h funciones_tp2.c mensajes.h strutil.c strutil.h zyxcba.c
	./pruebas_zyxcba/pruebas.sh ./zyxcba

.PHONY: zyxcba