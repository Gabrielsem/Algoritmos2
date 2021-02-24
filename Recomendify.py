from Grafo import Grafo
import Grafo_util
from lectura_datos import cargar_datos
import sys

CMD_CAMINO = "camino"
CMD_CANCIONES_IMPORTANTES = "mas_importantes"
CMD_RECOMENDACION = "recomendacion"
CMD_CICLO = "ciclo"
CMD_RANGO = "rango"
CMD_CLUSTERING = "clustering"

def camino(parametros):
	pass

def procesar_entrada(linea):
	entrada = linea.split(" ")
	if entrada[0] == CMD_CAMINO:
		camino(entrada[1:])
	elif entrada[0] == CMD_CANCIONES_IMPORTANTES:
		pass
	elif entrada[0] == CMD_RECOMENDACION:
		pass
	elif entrada[0] == CMD_CICLO:
		pass
	elif entrada[0] == CMD_RANGO:
		pass
	elif entrada[0] == CMD_CLUSTERING:
		pass
	else:
		print("Error: comando inválido (\"" + entrada[0] + "\")")
		return

def main():
	if len(sys.argv) < 2:
		print("Error: no se pasó ruta del archivo de datos")
		return

	usuarios_gustos = Grafo() # Grafo bipartito entre usuarios y canciones, conectándo a los usuarios con las canciones que les gustan
	canciones_similares = Grafo() # Grafo de canciones cuyas aristas conectan canciones similares
	cargar_datos(sys.argv[1], usuarios_gustos, canciones_similares)

	for linea in sys.stdin:
		procesar_entrada(linea.rstrip("\n"))

if __name__ == "__main__":
	main()