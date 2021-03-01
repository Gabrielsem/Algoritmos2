from Grafo import Grafo
import funciones_tp3
import Grafo_util
import lectura_datos 
import sys

from datetime import datetime

CMD_CAMINO = "camino"
CMD_CANCIONES_IMPORTANTES = "mas_importantes"
CMD_RECOMENDACION = "recomendacion"
CMD_CICLO = "ciclo"
CMD_RANGO = "rango"
CMD_CLUSTERING = "clustering"
CMD_INVALIDO = "ERROR: comando inválido ({})"

def procesar_entrada(linea, usuarios_gustos, canciones_similares, playlists):
	comando, _, parametros = linea.partition(" ")
	if comando == CMD_CAMINO:
		funciones_tp3.camino(parametros, usuarios_gustos)

	elif comando == CMD_CANCIONES_IMPORTANTES:
		pass

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

	elif comando == "temp_top_canciones":
		lectura_datos.grafo_canciones(playlists, canciones_similares)

		print("Fin carga de datos en grafo de canciones: ", datetime.now().time())


		pagerank = Grafo_util.pagerank(canciones_similares)
		print("Fin cálculo de pagerank: ", datetime.now().time())
		top = sorted(pagerank.items(), reverse = True, key=lambda x: x[1])
		print("Fin ordenar todos los valores de pagerank: ", datetime.now().time())
		for c in top:
			print(c)

	else:
		print(CMD_INVALIDO.format(comando))
		return

def main():
	if len(sys.argv) < 2:
		print("Error: no se pasó ruta del archivo de datos")
		return

	print("Inicio programa: ", datetime.now().time())

	canciones_similares = Grafo() # Grafo de canciones cuyas aristas conectan canciones similares (se arma si se necesita)
	usuarios_gustos, playlists = lectura_datos.leer_archivo(sys.argv[1])
	# \-> Grafo bipartito entre usuarios y canciones, conectándo a los usuarios con las canciones que les gustan

	print("Fin carga de datos inicial: ", datetime.now().time())

	for linea in sys.stdin:
		procesar_entrada(linea.rstrip("\n"), usuarios_gustos, canciones_similares, playlists)

if __name__ == "__main__":
	main()
