
zyxcba: dependencias/* colapac.c colapac.h csv.c csv.h funciones_tp2.h funciones_tp2.c mensajes.h zyxcba.c
	gcc -g -Wall -Wconversion -Wtype-limits -pedantic -Werror -o zyxcba dependencias/* colapac.c colapac.h csv.c csv.h funciones_tp2.h funciones_tp2.c mensajes.h zyxcba.c
	(cd pruebas_zyxcba; ./pruebas.sh ../zyxcba)

.PHONY: zyxcba