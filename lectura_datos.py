import csv
from Grafo import Grafo

FORMATO_CANCION = "{} - {}"
COL_CANCION = "TRACK_NAME"
COL_ARTISTA = "ARTIST"
COL_PLAYLIST = "PLAYLIST_NAME"
COL_USUARIO = "USER_ID"

def agregar_grafo_usuarios(usuarios_gustos, playlist_nombre, usuario, cancion):
	usuarios_gustos.agregar_vertice(usuario)
	usuarios_gustos.agregar_vertice(cancion)
	usuarios_gustos.agregar_arista(usuario, cancion, peso = playlist_nombre)

def agregar_dic_playlists(canciones_similares, playlists, playlist_actual, cancion_actual):
	if playlist_actual not in playlists:
		playlists[playlist_actual] = [cancion_actual]
	else:
		playlists[playlist_actual].append(cancion_actual)

# Devuelve tupla:
# 1. Grafo bipartito de usuarios y canciones, con aristas entre los usuarios y las canciones
# que esten en sus playlists
# 2. Diccionario de playlists (Clave = nombre de playlist, dato = lista de todas las canciones)
# Ese diccionario se puede pasar a grafo_canciones() para crear el Grafo correspondiente.
def leer_archivo(ruta_archivo, usuarios_gustos, canciones_similares):
	playlists = {} # Diccionario de playlists, guarda listas con las canciones de la playlist

	with open(ruta_archivo, "r") as archivo:
		lector = csv.DictReader(archivo, delimiter = '\t', quoting = csv.QUOTE_NONE)
		for fila in lector:
			cancion = FORMATO_CANCION.format(fila[COL_CANCION], fila[COL_ARTISTA])

			agregar_grafo_usuarios(usuarios_gustos, fila["PLAYLIST_NAME"], fila["USER_ID"], cancion)
			agregar_dic_playlists(canciones_similares, playlists, fila["PLAYLIST_ID"], cancion)

# Devuelve un grafo con vértices de canciones y aristas entre canciones que esten en una misma playlist.
# Recibe diccionario de playlists (Clave = nombre de playlist, dato = lista de todas las canciones)
def grafo_canciones(playlists):
	grafo = Grafo()
	for playlist in playlists:
		agregados = 0
		for cancion in playlist:
			grafo.agregar_vertice(cancion)
			agregados += 1
			for i in range(0, agregados - 1):
				grafo.agregar_arista(playlist[i], cancion)
