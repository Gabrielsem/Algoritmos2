import random

class Grafo:

	def __init__(self):
		""" crea un grafo con un diccionario de diccionario"""
		self.vertices = {}

	def agregar_vertice(self, vertice):
		"""agrega un vertice y devuelve false en caso de que el 
		vertice ya se encuentre en el grafo, caso contrario true"""
		if vertice in self.vertices:
			return False
		self.vertices[vertice] = {}
		return True

	def agregar_arista(self, v1, v2, peso = 1, dirigida = False):
		"""agrega una arista. En caso de ser dirigida v1 --> v2 y devuelve 
		true si se pudo agregar, en caso de que no se encuentre alguno de 
		los vertices devuelve false"""
		if not v1 in self.vertices or not v2 in self.vertices:
			return False

		self.vertices[v1][v2] = peso

		if not dirigida:
			self.vertices[v2][v1] = peso

		return True

	def quitar_vertice(self, v):
		"""quita el vertice del grafo y de todas las aristas con otros vertices """
		self.vertices.pop(v)
		for vertice, aristas in self.vertices.items():
			if v in aristas:
				aristas.pop(v)

	def quitar_arista(self, v1, v2, dirigida = False):
		"""quita una arista. En caso de ser dirigida v1 -X-> v2 """
		self.vertices[v1].pop(v2)
		if not dirigida:
			self.vertices[v2].pop(v1)


	def estan_unidos(self,v1,v2):
		"""devuelve true si v1 --> v2 """
		if v2 in self.vertices[v1]:
			return True
		return False

	def obtener_peso(self, v1, v2):
		return self.vertices[v1][v2]

	def existe_vertice(self, v):
		if v in self.vertices:
			return True
		return False

	def vertice_random(self):
		return random.choice(list(self.vertices.keys()))

	def vertices(self):
		"""devuelve una lista de todos los vertices """
		return list(self.vertices.keys())

	def adyacentes(self, v):
		"""devuelve una lista con los vertices adyacentes """
		return list(self.vertices[v].keys())

	def __iter__(self):
		return iter(self.vertices.keys())

	def __len__(self):
		return len(self.vertices)