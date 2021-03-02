from Grafo import Grafo
import funciones_tp3
import Grafo_util
import lectura_datos 
import sys

CMD_CAMINO = "camino"
CMD_CANCIONES_IMPORTANTES = "mas_importantes"
CMD_RECOMENDACION = "recomendacion"
CMD_CICLO = "ciclo"
CMD_RANGO = "rango"
CMD_CLUSTERING = "clustering"
CMD_INVALIDO = "ERROR: comando inválido ({})"

def procesar_entrada(linea, usuarios_gustos, canciones_similares, playlists, top_canciones):
	comando, _, parametros = linea.partition(" ")
	if comando == CMD_CAMINO:
		funciones_tp3.camino(parametros, usuarios_gustos)

	elif comando == CMD_CANCIONES_IMPORTANTES:
		lectura_datos.grafo_canciones(playlists, canciones_similares)
		
		funciones_tp3.canciones_importantes(parametros, canciones_similares, top_canciones)

	elif comando == CMD_RECOMENDACION:
		pass

	elif comando == CMD_CICLO:
		lectura_datos.grafo_canciones(playlists, canciones_similares)

		funciones_tp3.ciclo(parametros, canciones_similares)

	elif comando == CMD_RANGO:
		lectura_datos.grafo_canciones(playlists, canciones_similares)

		funciones_tp3.rango(parametros, canciones_similares)

	elif comando == CMD_CLUSTERING:
		pass
			
	else:
		print(CMD_INVALIDO.format(comando))
		return

def main():
	if len(sys.argv) < 2:
		print("Error: no se pasó ruta del archivo de datos")
		return

	canciones_similares = Grafo() # Grafo de canciones cuyas aristas conectan canciones similares (se arma si se necesita)
	usuarios_gustos, playlists = lectura_datos.leer_archivo(sys.argv[1])
	# \-> Grafo bipartito entre usuarios y canciones, conectándo a los usuarios con las canciones que les gustan

	top_canciones = ([], [])
	# Cuando se cargue el top_canciones, va a tener:
	# Lista 1: k canciones con pagerank más alto ordenados de mayor a menor
	# Lista 2: c-k tuplas (pagerank, cancion) con condicion de heap de maximos
	# (en realidad, se guarda el -1*pagerank y es un heap de mínimos, para usar el de python)

	for linea in sys.stdin:
		procesar_entrada(linea.rstrip("\n"), usuarios_gustos, canciones_similares, playlists, top_canciones)

if __name__ == "__main__":
	main()
