import Grafo_util
CAM_SEPARADOR = " >>>> "
CAM_ERROR = "Tanto el origen como el destino deben ser canciones."
CAM_MENSAJE = "{} --> aparece en la playlist --> {} ---> de ---> {} --> tiene una playlist --> {} --> donde aparece --> "

FLECHA = " --> "
SIN_RECORRIDO = "No se encontro recorrido"
NO_CANCION = "{} no es una canción válida."
NO_NUMERO = "{} no es una cantidad válida."


def camino(parametros, usuarios_gustos, canciones_similares):
	origen, _, destino = parametros.partition(CAM_SEPARADOR)
	if origen not in canciones_similares or destino not in canciones_similares:
		print(CAM_ERROR)
		return

	padres, _ = Grafo_util.recorrido_bfs(usuarios_gustos, destino, v_corte = origen)
	if origen not in padres:
		print(CAM_SIN_RECORRIDO)
		return
	
	cancion_actual = origen
	while not cancion_actual == destino:
		usuario = padres[cancion_actual]
		cancion_siguiente = padres[usuario]
		playlist_1 = usuarios_gustos.obtener_peso(cancion_actual, usuario)
		playlist_2 = usuarios_gustos.obtener_peso(usuario, cancion_siguiente)
		print(CAM_MENSAJE.format(cancion_actual, playlist_1, usuario, playlist_2), end = '') 
		cancion_actual = cancion_siguiente
	print(destino)

def ciclo(parametros, canciones_similares):
	n, _, cancion_origen = parametros.partition(" ")
	if not n.isdigit():
		print(NO_NUMERO.format(n))
		return
	if cancion_origen not in canciones_similares:
		print(NO_CANCION.format(cancion))
		return

	canciones = Grafo_util.ciclo_n(canciones_similares, cancion_origen, int(n))
	if len(canciones) == 0:
		print(SIN_RECORRIDO)
		return

	print(cancion_origen, end = '')
	for c in canciones:
		print(FLECHA + c, end = '')
	print("")