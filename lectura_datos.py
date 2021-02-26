import csv
from Grafo import Grafo

FORMATO_CANCION = "{} - {}"
COL_CANCION = "TRACK_NAME"
COL_ARTISTA = "ARTIST"
COL_PLAYLIST = "PLAYLIST_NAME"
COL_USUARIO = "USER_ID"

def agregar_gusto(usuarios_gustos, playlist_nombre, usuario, cancion):
	usuarios_gustos.agregar_vertice(usuario)
	usuarios_gustos.agregar_vertice(cancion)
	usuarios_gustos.agregar_arista(usuario, cancion, peso = playlist_nombre)

def agregar_similar(canciones_similares, playlists, playlist_actual, cancion_actual):
	canciones_similares.agregar_vertice(cancion_actual)

	if playlist_actual not in playlists:
		playlists[playlist_actual] = [cancion_actual]
	else:
		for cancion_anterior in playlists[playlist_actual]:
			canciones_similares.agregar_arista(cancion_anterior, cancion_actual)
		playlists[playlist_actual].append(cancion_actual)

def cargar_datos(ruta_archivo, usuarios_gustos, canciones_similares):
	playlists = {} # Diccionario de playlists, guarda listas con las canciones de la playlist

	with open(ruta_archivo, "r") as archivo:
		lector = csv.DictReader(archivo, delimiter = '\t', quoting = csv.QUOTE_NONE)
		for fila in lector:
			cancion = FORMATO_CANCION.format(fila[COL_CANCION], fila[COL_ARTISTA])

			agregar_gusto(usuarios_gustos, fila[COL_PLAYLIST], fila[COL_USUARIO], cancion)
			agregar_similar(canciones_similares, playlists, fila[COL_PLAYLIST], cancion)