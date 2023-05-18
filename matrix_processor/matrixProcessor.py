from matrix import Matrix



class ProcessError(Exception):
	def __init__(self, message,pc):
		self.message = message
		self.line = pc
		self.file = "<?>"
		
	def __str__(self):
		return "Error during execution at code block #"+str(self.line)+" in "+str(self.file)+":\n"+str(self.message)




class ProcessEnd(Exception):
	def __init__(self, pc,code):
		self.line = pc
		self.file = "<?>"
		self.code = code
	def __str__(self):
		return "Processus terminated at "+str(self.file)+":"+str(self.line)


class MatrixProcessor:

	def __init__(self, code, ram=[], m_size=10, regs=10):
		self.code = code # liste de string représantant chauqe ligne du programme
		self.PC = 0	# Program Counter
		self.ram = {i:ram[i] for i in range(len(ram))} # ram est un dictionnaire pour permettre de save à n'importe quel endroit
		self.m_size = m_size	# la taille des matrices dans ce processeur
		self.regs = [Matrix(m_size) for _ in range(regs)]	# les registres
		self.NULL = Matrix.NULL(m_size)	# utillisé pour tester les flags
		self.operations = {
			# le jeu d'instruction sous la forme de `nom : (fct, type1, type2, ...)`
			# ou Type1,... sont les types des arguments de la fonction fct
			"add": (self.add, "reg", "reg", "reg"),
			"sub": (self.sub, "reg", "reg", "reg"),
			"mul": (self.mul, "reg", "reg", "reg"),
			"exit": (self.exit, "int"),
			"exit_val": (self.exit_val, "reg"),
			"nop": (self.nop, ),
			"load":
			(self.load, "reg", "ram"),	# charge une matrice de la ram dans un registre
			"load_at":
			(self.load_at, "reg", "reg"),
			"save":
			(self.save, "reg", "ram"),	# sauvegarde une matrice d'un registre dans la ram
			"save_at":
			(self.save_at, "reg", "reg"),
			"set_fill": (self.set_fill, "reg", "int"),
			"set_id": (self.set_id, "reg"),
			"set_val": (self.set_val, "reg","int","int","int"),
			"move": (self.move, "reg", "reg"),
			"jmp": (self.jmp, "int"),
			"jmp_null":
			(self.jmp_null, "reg", "int"),	# jump si le registre contiens self.NULL
			"print_reg": (self.print_reg, "reg"),
			"print_all": (self.print_all, ),
			"print_str": (self.print_str, "str"),
		}

	def iterate(self):	# fais une itération du programme
		if self.PC >= len(self.code): return "ERR: Out of range at line " + str(self.PC)
		ins = self.code[self.PC].split(" ")
		if not ins[0].lower() in self.operations.keys():
			raise ProcessError("Instruction " + ins[0] + " isn't valid",self.PC)
		if len(ins) != len(self.operations[ins[0].lower()]):
			raise ProcessError("Instruction " + ins[0] + " doesn't have the right number of arguments",self.PC)

		fct = self.operations[ins[0].lower()][0]
		types = self.operations[ins[0].lower()][1:]
		params = []

		for i, param in enumerate(ins[1:]):

			if types[i] == "str":
				params.append(param.replace("_"," "))

			elif types[i] == "int":
				try:
					params.append(int(param))
				except ValueError:
					raise ProcessError("Argument of " + str(ins[0]) +  " is not a int.",self.PC)

			elif types[i] == "ram":
				try:
					a = int(param)
				except ValueError:

					raise ProcessError("Argument of " + str(ins[0]) +  " is not a ram adress.",self.PC)
				if a < 0 or a >= len(self.ram):
					raise ProcessError("Argument of " + str(ins[0]) +  " is a Out-Of-Bound ram adress.",self.PC)
				else:
					params.append(a)

			elif types[i] == "reg":
				try:
					a = int(param)
				except ValueError:
					raise ProcessError("Argument of " + str(ins[0]) +  " is not a register index.",self.PC)
				if a < 0 or a >= len(self.regs):
					raise ProcessError("Argument of " + str(ins[0]) +  " is a Out-Of-Bound register index.",self.PC)
				else:
					params.append(a)
			else:
				raise ProcessError("Unknow Type `"+ str(types[i]) + "` for ins=" + str(ins[0]) + ".\nThat not the program's fault.",self.PC)

		self.PC += 1
		return fct(*params)

	def nop(self):return "Executing NOP (No OPerations)"

	def add(self, a, b, dest):
		self.regs[dest] = self.regs[a] + self.regs[b]
		return "Executing ADD on R" + str(a) + " + R" + str(b) + " -> R" + str(dest)

	def sub(self, a, b, dest):
		self.regs[dest] = self.regs[a] - self.regs[b]
		return "Executed SUB on R" + str(a) + " - R" + str(b) + " -> R" + str(dest)

	def mul(self, a, b, dest):
		self.regs[dest] = self.regs[a] * self.regs[b]
		return "Executed MUL on R" + str(a) + " * R" + str(b) + " -> R" + str(dest)

	def move(self, a, b):
		self.regs[a] = self.regs[b].copy()
		return "Executed MOVE on R" + str(a) + " <- R" + str(b)

	def save(self, a, i):
		self.ram[i] = self.regs[a].copy()
		return "Executed SAVE on RAM[" + str(i) + "] <- R" + str(a)
	
	def save_at(self, x, b):
		self.ram[self.regs[b].data[0][0]] = self.regs[x].copy()
		return "Executed SAVE_AT on RAM[R" + str(b) + ".0.0] <- R" + str(x)

	def load(self, a, i):
		self.regs[a] = self.ram.get(i,Matrix.NULL(self.m_size)).copy()
		return "Executed LOAD on R" + str(a) + " <- RAM[" + str(i) + "]"

	def load_at(self, a, b):
		self.regs[a] = self.ram.get(self.regs[b].data[0][0],Matrix.NULL(self.m_size)).copy()
		return "Executed LOAD_AT on R" + str(a) + " <- RAM[R" + str(b) + ".0.0]"

	def jmp(self, i):
		self.PC = i
		return "Executed JMP to " + str(i)

	def set_fill(self, a, value):
		self.regs[a] = Matrix.FILL(value, self.m_size, self.m_size)
		return "Executed SET_FILL on " + str(a) + " w/ " + str(value)

	def set_id(self, a):
		self.regs[a] = Matrix.IDENTITY(self.m_size)
		return "Executed SET_ID on R" + str(a)

	def set_val(self, a, y, x, val):
		self.regs[a].data[y][x] = val
		return "Executed SET_VAL on R" + str(a) + "["+str(y)+"]["+str(x)+"] <- v" + str(val)

	def exit(self,i):
		raise ProcessEnd(self.PC,i)
	
	def exit_val(self,i):
		raise ProcessEnd(self.PC,self.regs[i])

	def jmp_null(self, a, i):
		if self.regs[a] == self.NULL: self.jmp(i)
		return "Executed jmp_null if R" + str(a) + " = 0 (" + str(
		 self.regs[a] == self.NULL) + ") JMP to " + str(i)

	def print_reg(self, a):
		print(" --- DUMP OF REG[" + str(a) + "] ---")
		print(self.regs[a])
		return "Executed PRINT_REG on R" + str(a)

	def print_str(self,stri):
		print(stri)
		return "Executed PRINT_STR on string `" + stri+"`"

	def print_all(self):
		print(" --- print_all ---")
		print("self.PC = " + str(self.PC))
		for i, x in enumerate(self.regs):
			print("\nREG[" + str(i) + "]")
			print(x)
		return "Executed DEBUG"

