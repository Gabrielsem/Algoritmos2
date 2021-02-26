import Grafo_util

CAM_SEPARADOR = " >>>> "
CAM_ERROR = "Tanto el origen como el destino deben ser canciones."
CAM_MENSAJES = ["aparece en la playlist", "de", "tiene una playlist", "donde aparece"]

FLECHA = " --> "
SIN_RECORRIDO = "No se encontro recorrido"
NO_CANCION = "{} no es una canción válida."
NO_NUMERO = "{} no es una cantidad válida."

def imprimir_lista(lista, separador = ' ', mensajes_intermedios = []):
	i = 0
	for s in lista[:-1]:
		print(s, end = separador)
		if len(mensajes_intermedios) > 0:
			print(mensajes_intermedios[i], end = separador)
			i = (i + 1) % len(mensajes_intermedios)
	print(lista[-1])

def camino(parametros, usuarios_gustos, canciones_similares):
	origen, _, destino = parametros.partition(CAM_SEPARADOR)
	if origen not in canciones_similares or destino not in canciones_similares:
		print(CAM_ERROR)
		return

	camino_min = Grafo_util.camino_minimo(usuarios_gustos, origen, destino, con_peso = True)
	if len(camino_min) == 0:
		print(SIN_RECORRIDO)
		return
	
	imprimir_lista(camino_min, separador = FLECHA, mensajes_intermedios = CAM_MENSAJES)

def ciclo(parametros, canciones_similares):
	n, _, cancion = parametros.partition(" ")
	if not n.isdigit():
		print(NO_NUMERO.format(n))
		return
	if cancion not in canciones_similares:
		print(NO_CANCION.format(cancion))
		return

	lista_ciclo = Grafo_util.ciclo_n(canciones_similares, cancion, int(n))
	if len(lista_ciclo) == 0:
		print(SIN_RECORRIDO)
		return

	imprimir_lista(lista_ciclo, separador = FLECHA)

def rango(parametros, canciones_similares):
	n, _, cancion = parametros.partition(" ")
	if not n.isdigit():
		print(NO_NUMERO.format(n))
		return
	if cancion not in canciones_similares:
		print(NO_CANCION.format(cancion))
		return

	print(Grafo_util.rango(canciones_similares, cancion, int(n)))