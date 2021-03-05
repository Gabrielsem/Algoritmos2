#!/usr/bin/python3

from grafo import Grafo
import funciones_tp3
import lectura_datos
import sys
from mensajes import *

def procesar_entrada(linea, grafo_usuarios, grafo_canciones, playlists, top_canciones):
	comando, _, parametros = linea.partition(" ")

	if not (comando == CMD_CAMINO or comando == CMD_RECOMENDACION):
		 lectura_datos.cargar_grafo_canciones(playlists, grafo_canciones)

	if comando == CMD_CAMINO:
		funciones_tp3.camino(parametros, grafo_usuarios)

	elif comando == CMD_CANCIONES_IMPORTANTES:
		funciones_tp3.canciones_importantes(parametros, grafo_canciones, top_canciones)

	elif comando == CMD_RECOMENDACION:
		funciones_tp3.recomendacion(parametros, grafo_usuarios)

	elif comando == CMD_CICLO:
		funciones_tp3.ciclo(parametros, grafo_canciones)

	elif comando == CMD_RANGO:
		funciones_tp3.rango(parametros, grafo_canciones)

	elif comando == CMD_CLUSTERING:
		funciones_tp3.clustering(parametros, grafo_canciones)
		
	else:
		print(CMD_INVALIDO.format(comando))


def main():
	if len(sys.argv) < 2:
		print("Error: no se pasó ruta del archivo de datos")
		return

	grafo_canciones = Grafo() # Grafo de canciones cuyas aristas conectan canciones similares (se arma si se necesita)
	grafo_usuarios, playlists = lectura_datos.leer_archivo(sys.argv[1])
	# \-> Grafo bipartito entre usuarios y canciones, conectándo a los usuarios con las canciones que les gustan

	top_canciones = ([], [])
	# Cuando se ejecute la función de funciones_tp3.canciones_importantes(), va a tener:
	# Lista 1: k canciones con pagerank más alto ordenados de mayor a menor
	# Lista 2: c-k tuplas (pagerank, cancion) con condicion de heap de maximos
	# (en realidad, se guarda el -1*pagerank y es un heap de mínimos, para usar el del módulo heapq)

	for linea in sys.stdin:
		procesar_entrada(linea.rstrip("\n"), grafo_usuarios, grafo_canciones, playlists, top_canciones)

main()