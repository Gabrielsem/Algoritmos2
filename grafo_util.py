from grafo import Grafo
from collections import deque
import random

PR_MAX_ITER = 100
PR_TOL_ERR = 1.0e-3
PR_AMORT = 0.85
PRRW_MAX_ITER = 100
PRRW_LARGO_RW = 500

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

# Devuelve una lista con los elementos del camino mínimo entre v_origen y v_destino.
# Solo para grafos no pesados.
# La lista se devuelve vacía si no se encuentra camino
# Si con_peso es verdadero, la lista contendrá tuplas con los pesos correspondientes.
# ej. (v1, v2, peso(v1, v2)), (v2, v3, peso(v2, v3)), etc
def camino_minimo(grafo, v_origen, v_destino, con_peso = False):
	lista = []
	padres, _ = recorrido_bfs(grafo, v_destino, v_corte = v_origen)
	if v_origen not in padres:
		return lista

	actual = v_origen
	while not actual == None:
		sig = padres[actual]
		if not con_peso:
			lista.append(actual)
		else:
			if not sig:
				break
			lista.append((actual, sig, grafo.obtener_peso(actual, sig)))
		actual = sig

	return lista

def _ciclo_aux(grafo, origen, actual, n, visitados):
	if n <= 1:
		if grafo.estan_unidos(origen, actual):
			return [origen, actual]
		return []

	visitados.add(actual)
	for v in grafo.adyacentes(actual):
		if v in visitados:
			continue
		lista = _ciclo_aux(grafo, origen, v, n - 1, visitados)
		if len(lista) > 0:
			lista.append(actual)
			return lista
	
	visitados.remove(actual)
	return []

# Devuelve una lista de n + 1 vértices de un ciclo de n vértices,
# empezando y terminando en origen. La lista se devuelve vacía si no se encuentra
# tal ciclo.
def ciclo_n(grafo, origen, n):
	visitados = set()
	return _ciclo_aux(grafo, origen, origen, n, visitados)

# Devuelve la cantidad de vértices que se encuenten a exactamente n saltos
# del vértice pasado por parámetro.
def rango(grafo, vertice, n):
	_, ordenes = recorrido_bfs(grafo, vertice, orden_corte = n)
	cantidad = 0
	for v in ordenes:
		if ordenes[v] == n:
			cantidad += 1

	return cantidad

# devuelve el pagerank de los vertices del grafo pasado por parametro
# pr(pi) = (1-d)/N + d. EE pr(pj)/ L(pj)
def pagerank(grafo):
	N = len(grafo)
	nstart = 1/N
	d = PR_AMORT
	dic_prs = dict((x, nstart) for x in grafo)

	for i in range(PR_MAX_ITER):
		err = 0
		for vertice in grafo:
			aux = dic_prs[vertice]
			dic_prs[vertice] = (1-d) / N + d * sum(dic_prs[ady] / len(grafo.adyacentes(ady)) for ady in grafo.adyacentes(vertice))
			err += abs(dic_prs[vertice] - aux)

		if err < PR_TOL_ERR:
			return dic_prs
	return dic_prs


def _clustering_v(grafo, v):
	ady = grafo.adyacentes(v)
	grado_sal = len(ady)
	adyacentes_unidos = 0
	if grado_sal < 2:
		return 0

	for w in ady:
		for u in ady:
			if grafo.estan_unidos(w, u):
				adyacentes_unidos += 1

	return adyacentes_unidos/(grado_sal*(grado_sal - 1))

def clustering(grafo, vertice = None):
	if len(grafo) == 0:
		return 0
	if vertice:
		return _clustering_v(grafo, vertice)

	c_total = 0
	for v in grafo:
		c_total += _clustering_v(grafo, v)

	return c_total/len(grafo)

#a_recomendar es el parametro de si tiene que recomendar canciones o usuarios
def pr_rand_walk(grafo, lista_origenes):
	dic_prs = dict((x, 0) for x in grafo)

	for i in range(PRRW_MAX_ITER):
		val_pr = 1 #valor con que se empieza a asignar prs
		dic_actual = {}
		actual = random.choice(lista_origenes)
		for L in range(PRRW_LARGO_RW):
			lista_ady = grafo.adyacentes(actual)
			actual = random.choice(lista_ady)
			len_ady = len(lista_ady)
			dic_actual[actual] = dic_actual.get(actual, 0) + val_pr / len_ady
			val_pr = dic_actual[actual]
		sumar_dics(dic_prs, dic_actual)

	return dic_prs


def sumar_dics(dic_total, dic_actual):
	for key, val in dic_actual.items():
		dic_total[key] += val
