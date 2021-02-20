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


	