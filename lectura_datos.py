import csv
from Grafo import Grafo


def agregar_gusto(usuarios_gustos, usuario, cancion):
	usuarios_gustos.agregar_vertice(usuario)
	usuarios_gustos.agregar_vertice(cancion)
	usuarios_gustos.agregar_arista(usuario, cancion)

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
			cancion = fila["TRACK_NAME"] + " - " + fila["ARTIST"]

			agregar_gusto(usuarios_gustos, fila["USER_ID"], cancion)
			agregar_similar(canciones_similares, playlists, fila["PLAYLIST_ID"], cancion)