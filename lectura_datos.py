import csv
from Grafo import Grafo

FORMATO_CANCION = "{} - {}"
COL_CANCION = "TRACK_NAME"
COL_ARTISTA = "ARTIST"
COL_PLAYLIST = "PLAYLIST_NAME"
COL_USUARIO = "USER_ID"

ID_USUARIO = "U"
ID_CANCION = "C"

def agregar_grafo_usuarios(usuarios_gustos, playlist_nombre, usuario, cancion):
	usuarios_gustos.agregar_vertice(usuario)
	usuarios_gustos.agregar_vertice(cancion)
	usuarios_gustos.agregar_arista(usuario, cancion, peso = playlist_nombre)

def agregar_dic_playlists(playlists, playlist_actual, cancion_actual):
	if playlist_actual not in playlists:
		playlists[playlist_actual] = [cancion_actual]
	else:
		playlists[playlist_actual].append(cancion_actual)

# Devuelve tupla:
# 1. Grafo bipartito de usuarios y canciones, con aristas entre los usuarios y las canciones
# que esten en sus playlists
# 2. Diccionario de playlists (Clave = nombre de playlist, dato = lista de todas las canciones)
# Ese diccionario se puede pasar a grafo_canciones() para crear el Grafo correspondiente.
def leer_archivo(ruta_archivo):
	playlists = {} # Diccionario de playlists, guarda listas con las canciones de la playlist
	usuarios_gustos = Grafo()

	with open(ruta_archivo, "r") as archivo:
		lector = csv.DictReader(archivo, delimiter = '\t', quoting = csv.QUOTE_NONE)
		for fila in lector:
			cancion = FORMATO_CANCION.format(fila[COL_CANCION], fila[COL_ARTISTA])
			usuario = fila[COL_USUARIO]
			playlist = fila[COL_PLAYLIST]

			agregar_grafo_usuarios(usuarios_gustos, playlist, (ID_USUARIO, usuario), (ID_CANCION, cancion))
			agregar_dic_playlists(playlists, playlist, cancion)

	return usuarios_gustos, playlists

# Carga los datos del grafo con vértices de canciones y aristas entre canciones que esten en una misma playlist.
# Si ya esta cargado, no hace nada
# Recibe diccionario de playlists (Clave = nombre de playlist, dato = lista de todas las canciones)
# Se borran los elementos del diccionario al utilizar esta función
def grafo_canciones(playlists, canciones_similares):
	if len(canciones_similares) > 0:
		return

	for playlist in playlists.values():
		for c in playlist:
			canciones_similares.agregar_vertice(c)

		while(len(playlist) > 0):
			c_actual = playlist.pop()
			for c in playlist:
				canciones_similares.agregar_arista(c_actual, c)
	playlists.clear()
