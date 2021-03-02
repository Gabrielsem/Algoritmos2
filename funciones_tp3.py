import Grafo_util
from lectura_datos import ID_CANCION, ID_USUARIO
import heapq

CAM_SEPARADOR = " >>>> "
CAM_ERROR = "Tanto el origen como el destino deben ser canciones."
CAM_MENSAJES = ["aparece en la playlist", "de", "tiene una playlist", "donde aparece"]

FLECHA = " --> "
SIN_RECORRIDO = "No se encontro recorrido"
NO_CANCION = "{} no es una canción válida."
NO_NUMERO = "{} no es una cantidad válida."

TOPC_SEPARADOR = "; "

CLUST_FORMATO = "{:.3f}"

def mensaje_camino(camino):
	msj = [None]*(len(camino)*4)
	# x tiene forma ((v1, v2, peso)) con v = (ID_CANCION/ID_USUARIO, nombre)
	msj[::4] = [x[0][1] for x in camino] # Agrego los nombres de los v1
	msj[2::4] = [x[2] for x in camino] # Agrego los pesos (playlists)
	msj[1::2] = CAM_MENSAJES*int(len(camino)/2) # Agrego las partes constantes del mensaje
	msj.append(camino[-1][1][1]) # Agrego el nombre del ultimo v2
	return msj

def camino(parametros, grafo_usuarios):
	origen, _, destino = parametros.partition(CAM_SEPARADOR)
	origen = (ID_CANCION, origen)
	destino = (ID_CANCION, destino)
	if origen not in grafo_usuarios or destino not in grafo_usuarios:
		print(CAM_ERROR)
		return

	camino_min = Grafo_util.camino_minimo(grafo_usuarios, origen, destino, con_peso = True)
	if len(camino_min) == 0:
		print(SIN_RECORRIDO)
		return

	print(*mensaje_camino(camino_min), sep = FLECHA)

def ciclo(parametros, grafo_canciones):
	n, _, cancion = parametros.partition(" ")
	if not n.isdigit():
		print(NO_NUMERO.format(n))
		return
	if cancion not in grafo_canciones:
		print(NO_CANCION.format(cancion))
		return

	lista_ciclo = Grafo_util.ciclo_n(grafo_canciones, cancion, int(n))
	if len(lista_ciclo) == 0:
		print(SIN_RECORRIDO)
		return

	print(*lista_ciclo, sep = FLECHA)

def rango(parametros, grafo_canciones):
	n, _, cancion = parametros.partition(" ")
	if not n.isdigit():
		print(NO_NUMERO.format(n))
		return
	if cancion not in grafo_canciones:
		print(NO_CANCION.format(cancion))
		return

	print(Grafo_util.rango(grafo_canciones, cancion, int(n)))

def canciones_importantes(parametros, grafo_canciones, top_canciones):
	n, _, _ = parametros.partition(" ")
	if not n.isdigit():
		print(NO_NUMERO.format(n))
		return
	n = int(n)

	cant_canciones = len(top_canciones[0]) + len(top_canciones[1])
	if cant_canciones == 0:
		top_canciones[1].extend([(-1*pr, c) for c, pr in Grafo_util.pagerank(grafo_canciones).items()]) 
		heapq.heapify(top_canciones[1])
		cant_canciones = len(top_canciones[1])

	if cant_canciones < n:
		n = cant_canciones

	if len(top_canciones[0]) < n:
		cant_desencolar = n - len(top_canciones[0])
		for i in range(0, cant_desencolar):
			_, cancion = heapq.heappop(top_canciones[1])
			top_canciones[0].append(cancion)

	print(*top_canciones[0][:n], sep = TOPC_SEPARADOR)

def clustering(parametros, grafo_canciones):
	if parametros == "":
		print(CLUST_FORMATO.format(float(Grafo_util.clustering(grafo_canciones))))
		return

	if parametros not in grafo_canciones:
		print(NO_CANCION.format(parametros))
		return

	print(CLUST_FORMATO.format(float(Grafo_util.clustering(grafo_canciones, vertice = parametros))))