class Matrix:  # Un simple type Matrice avec les opérations d'addition, soustraction etc...

	def __init__(self, X=10, Y=10):
		self.x = X  # Dimension en X
		self.y = Y  # Dimension en Y
		self.data = [[0 for _ in range(X)] for _ in range(Y)]

	def __add__(self, other):
		assert self.x == other.x
		assert self.y == other.y
		out = Matrix(self.x, self.y)
		for y in range(self.y):
			for x in range(self.x):
				out.data[y][x] = self.data[y][x] + other.data[y][x]
		return out

	def __sub__(self, other):
		assert self.x == other.x
		assert self.y == other.y
		out = Matrix(self.x, self.y)
		for y in range(self.y):
			for x in range(self.x):
				out.data[y][x] = self.data[y][x] - other.data[y][x]
		return out

	def __mul__(self, other):
		if isinstance(other, int):  # si on fais Matrice * Scalaire
			out = Matrix(self.x, self.y)
			for y in range(self.y):
				for x in range(self.x):
					out.data[y][x] = other * self.data[x][y]
			return out
		elif isinstance(other, Matrix):  # si on fais Matrice * Matrice
			assert other.y == self.x
			out = Matrix(other.x, self.y)
			for x in range(other.x):
				for y in range(self.y):
					out.data[y][x] = sum(
					 [self.data[y][i] * other.data[i][x] for i in range(self.x)])
			return out

	def __rmul__(self, other):
		if isinstance(other, int):  # Si on fais Scalaire * Matrice
			return self.__mul__(other)

	def __eq__(self, other):
		if self.x != other.x or self.y != other.y: return False
		return all([
		 all([self.data[x][y] == other.data[x][y] for y in range(self.y)])
		 for x in range(self.x)
		])

	def __str__(self):
		return "\n".join([
		 " ".join(['{:2.2} '.format(str(self.data[i][j])) for j in range(self.x)])
		 for i in range(self.y)
		])

	# génère une matrice depuis un tableau à 2 dimensions
	@staticmethod
	def from_table(table):
		assert len(table) >= 1 and len(table[0]) >= 1
		out = Matrix(len(table[0]), len(table))
		out.data = table
		return out

	# génère une matrice Identité de taille n
	@staticmethod
	def IDENTITY(n):
		ret = Matrix(n, n)
		for i in range(n):
			ret.data[i][i] = 1
		return ret

	# génère une matrice remplie de `value` de taille x*y
	@staticmethod
	def FILL(value, x, y):
		ret = Matrix(x, y)
		ret.data = [[value for _ in range(x)] for _ in range(y)]
		return ret

	# génère une matrice carré vide de taille n
	@staticmethod
	def NULL(n):
		return Matrix.FILL(0, n, n)

	def copy(self):
		return Matrix.from_table([x.copy() for x in self.data])


def to_matrix(txt):	# petite fonction pour transformer du texte sous la forme donné par https://csacademy.com/app/graph_editor/ en matrice
	n = max(
	 [max(int(s.split(" ")[0]), int(s.split(" ")[1]))
		for s in txt.split("\n")]) + 1
	out = Matrix(n, n)
	for l in txt.split("\n"):
		out.data[int(l.split(" ")[0])][int(l.split(" ")[1])] = 1
	return out

def test_matrix():
	A = 5 * Matrix.IDENTITY(5)
	B = Matrix.FILL(2, 5, 5)
	C = Matrix.from_table([[0, 1, 2], [3, 4, 5]])
	D = Matrix.from_table([[1, 2], [3, 4], [5, 6]])
	print(" - test 1 -")
	print(A + B * B - 2 * A)
	print(" - test 2 - ")
	print(C * (D + Matrix.FILL(1, 2, 3)) - 2 * Matrix.FILL(2, 2, 2))


if __name__ == "__main__":
	test_matrix()