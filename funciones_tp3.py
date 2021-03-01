import Grafo_util
from lectura_datos import ID_CANCION, ID_USUARIO

CAM_SEPARADOR = " >>>> "
CAM_ERROR = "Tanto el origen como el destino deben ser canciones."
CAM_MENSAJES = ["aparece en la playlist", "de", "tiene una playlist", "donde aparece"]

FLECHA = " --> "
SIN_RECORRIDO = "No se encontro recorrido"
NO_CANCION = "{} no es una canción válida."
NO_NUMERO = "{} no es una cantidad válida."

def mensaje_camino(camino):
	msj = [None]*(len(camino)*4)
	# x tiene forma ((v1, v2, peso)) con v = (ID_CANCION/ID_USUARIO, nombre)
	msj[::4] = [x[0][1] for x in camino] # Agrego los nombres de los v1
	msj[2::4] = [x[2] for x in camino] # Agrego los pesos (playlists)
	msj[1::2] = CAM_MENSAJES*int(len(camino)/2) # Agrego las partes constantes del mensaje
	msj.append(camino[-1][1][1]) # Agrego el nombre del ultimo v2
	return msj

def camino(parametros, usuarios_gustos):
	origen, _, destino = parametros.partition(CAM_SEPARADOR)
	origen = (ID_CANCION, origen)
	destino = (ID_CANCION, destino)
	if origen not in usuarios_gustos or destino not in usuarios_gustos:
		print(CAM_ERROR)
		return

	camino_min = Grafo_util.camino_minimo(usuarios_gustos, origen, destino, con_peso = True)
	if len(camino_min) == 0:
		print(SIN_RECORRIDO)
		return
	print(*mensaje_camino(camino_min), sep = FLECHA)

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

	print(*lista_ciclo, sep = FLECHA)

def rango(parametros, canciones_similares):
	n, _, cancion = parametros.partition(" ")
	if not n.isdigit():
		print(NO_NUMERO.format(n))
		return
	if cancion not in canciones_similares:
		print(NO_CANCION.format(cancion))
		return

	print(Grafo_util.rango(canciones_similares, cancion, int(n)))