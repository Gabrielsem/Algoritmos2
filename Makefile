zyxcba: dependencias/* paciente.c paciente.h colapac.c colapac.h clinica.c clinica.h lectura_archivos.c lectura_archivos.h funciones_tp2.h funciones_tp2.c mensajes.h zyxcba.c
	gcc -g -Wall -Wconversion -Wtype-limits -pedantic -Werror -o zyxcba dependencias/* paciente.c paciente.h colapac.c colapac.h clinica.c clinica.h lectura_archivos.c lectura_archivos.h funciones_tp2.h funciones_tp2.c mensajes.h zyxcba.c
	(cd pruebas_zyxcba; ./pruebas.sh ../zyxcba)

.PHONY: zyxcba