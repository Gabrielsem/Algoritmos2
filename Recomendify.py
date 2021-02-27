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

def procesar_entrada(linea, usuarios_gustos, canciones_similares, playlists):
	comando, _, parametros = linea.partition(" ")
	if comando == CMD_CAMINO:
		if canciones_similares is None: 
			canciones_similares = lectura_datos.grafo_canciones(playlists)

		funciones_tp3.camino(parametros, usuarios_gustos, canciones_similares)
	elif comando == CMD_CANCIONES_IMPORTANTES:
		pass
	elif comando == CMD_RECOMENDACION:
		pass
	elif comando == CMD_CICLO:
		if canciones_similares is None: 
			canciones_similares = lectura_datos.grafo_canciones(playlists)

		funciones_tp3.ciclo(parametros, canciones_similares)
	elif comando == CMD_RANGO:
		if canciones_similares is None: 
			canciones_similares = lectura_datos.grafo_canciones(playlists)

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

	canciones_similares = None # Grafo de canciones cuyas aristas conectan canciones similares (se arma si se necesita)
	usuarios_gustos, playlists = lectura_datos.leer_archivo(sys.argv[1])
	# \-> Grafo bipartito entre usuarios y canciones, conectándo a los usuarios con las canciones que les gustan

	for linea in sys.stdin:
		procesar_entrada(linea.rstrip("\n"), usuarios_gustos, canciones_similares, playlists)

if __name__ == "__main__":
	main()