from Grafo import Grafo
from collections import deque

# Hace un recorrido BFS desde el vértice de origen indicado. 
# Devuelve los diccionarios de padres y ordenes, en ese orden
# Si se indica un vértice de corte, se corta al encontrar ese vértice.
# Si se indica orden de corte, se recorren todos los vértices hasta ese orden inclusive.
def recorrido_bfs(grafo, v_origen, v_corte = None, orden_corte = float("inf")):
	cola = deque([v_origen])
	padres = {v_origen:None}
	ordenes = {v_origen:0}
	if v_origen == v_corte:
		return padres, ordenes

	while(len(cola) > 0):
		v = cola.pop()
		for w in grafo.adyacentes(v):
			if w in ordenes:
				continue
			padres[w] = v
			ordenes[w] = ordenes[v] + 1

			if w == v_corte:
				return padres, ordenes

			if ordenes[w] < orden_corte:
				cola.appendleft(w)

	return padres, ordenes

def ciclo_aux(grafo, origen, actual, n, visitados):
	if n <= 1:
		if grafo.estan_unidos(origen, actual):
			return [actual]
		return []

	visitados.add(actual)
	for v in grafo.adyacentes(actual):
		if v in visitados:
			continue
		lista = ciclo_aux(grafo, origen, v, n - 1, visitados)
		if len(lista) > 0:
			lista.append(actual)
			return lista
	
	visitados.remove(actual)
	return []

# Devuelve una lista de los vértices de un ciclo de n elementos,
# terminando en origen. La lista se devuelve vacía si no se encuentra
# tal ciclo.
def ciclo_n(grafo, origen, n):
	visitados = set()
	return ciclo_aux(grafo, origen, origen, n, visitados)

