import Grafo_util
CAM_SEPARADOR = " >>>> "
CAM_SEPARADOR_SALIDA = " --> "
CAM_ERROR = "Tanto el origen como el destino deben ser canciones."
CAM_MENSAJE = "{} --> aparece en la playlist --> {} ---> de ---> {} --> tiene una playlist --> {} --> donde aparece --> "
CAM_SIN_RECORRIDO = "No se encontro recorrido"

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