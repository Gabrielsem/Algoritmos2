from Grafo import Grafo
import funciones_tp3
from lectura_datos import cargar_datos
import sys

CMD_CAMINO = "camino"
CMD_CANCIONES_IMPORTANTES = "mas_importantes"
CMD_RECOMENDACION = "recomendacion"
CMD_CICLO = "ciclo"
CMD_RANGO = "rango"
CMD_CLUSTERING = "clustering"

CMD_INVALIDO = "ERROR: comando inválido ({})"

def procesar_entrada(linea, usuarios_gustos, canciones_similares):
	comando, _, parametros = linea.partition(" ")
	if comando == CMD_CAMINO:
		funciones_tp3.camino(parametros, usuarios_gustos, canciones_similares)
	elif comando == CMD_CANCIONES_IMPORTANTES:
		pass
	elif comando == CMD_RECOMENDACION:
		pass
	elif comando == CMD_CICLO:
		funciones_tp3.ciclo(parametros, canciones_similares)
	elif comando == CMD_RANGO:
		pass
	elif comando == CMD_CLUSTERING:
		pass
	else:
		print(CMD_INVALIDO.format(comando))
		return

def main():
	if len(sys.argv) < 2:
		print("Error: no se pasó ruta del archivo de datos")
		return

	usuarios_gustos = Grafo() # Grafo bipartito entre usuarios y canciones, conectándo a los usuarios con las canciones que les gustan
	canciones_similares = Grafo() # Grafo de canciones cuyas aristas conectan canciones similares
	cargar_datos(sys.argv[1], usuarios_gustos, canciones_similares)

	for linea in sys.stdin:
		procesar_entrada(linea.rstrip("\n"), usuarios_gustos, canciones_similares)

if __name__ == "__main__":
	main()