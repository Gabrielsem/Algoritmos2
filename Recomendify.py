from Grafo import Grafo
import Grafo_util
from lectura_datos import leer_archivo
import sys

def main():
	if len(sys.argv) < 2:
		print("Error: no se pasó ruta del archivo de datos")
		return

	usuarios_gustos = Grafo() # Grafo bipartito entre usuarios y canciones, conectándo a los usuarios con las canciones que les gustan
	canciones_similares = Grafo() # Grafo de canciones cuyas aristas conectan canciones similares
	leer_archivo(sys.argv[1], usuarios_gustos, canciones_similares)

if __name__ == "__main__":
	main()
