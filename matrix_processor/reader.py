from matrixProcessor import MatrixProcessor, ProcessError, ProcessEnd
from matrix import Matrix

class ParserError(Exception):
	def __init__(self, section, message):
		self.section = section
		self.message = message
		self.file = "<?>"
		self.line = "<?>"
		
	def __str__(self):
		return "Error during parsing of section `"+str(self.section)+"` in "+str(self.file)+":"+str(self.line)+":\n"+str(self.message)


class Interpretor:

	def __init__(self,file):
		file_obj = open(file,"r")
		active_section = None
		line_nb = -1

		self.code = {} # dict in python have an order
		self.active_code_sect = "start"
		self.info = {
			"set_entry":"start",
			"nb_reg":10,
			"size":None,
			"debug":True
		}
		self.arguments = []
		self.ram = []

		for line in file_obj.readlines():
			line_nb+=1
			line = line.split(";")[0].strip().split(" ")
			if line == [] or line == [""]:continue
			if line[0][0] == ".":
				active_section = line[0][1:].lower()
				continue
			else:
				if active_section == "code":
					try:
						self.parse_code_section(line)
					except ParserError as psr:
						psr.line = line_nb
						psr.file = file
						raise psr
				elif active_section == "info":
					try:
						self.parse_info_section(line)
					except ParserError as psr:
						psr.line = line_nb
						psr.file = file
						raise psr
				elif active_section == "ram":
					try:
						self.parse_ram_section(line)
					except ParserError as psr:
						psr.line = line_nb
						psr.file = file
						raise psr
						
				else:
					psr = ParserError("Unknow","Unknow section `"+str(active_section)+"`")
					psr.line = line_nb
					psr.file = file
					raise psr

		file_obj.close()
	
	def run(self,argument_list):
		if not self.info["set_entry"] in self.code.keys():
			raise ParserError("code","No entry point for the code in the `info` section.")
		
		for mat in argument_list:
			if not isinstance(mat,Matrix):
				raise ParserError("ram","Arguments must be of type matrix.")
			elif mat.x != mat.y:
				raise ParserError("ram","Arguments must be square.")

		code = ["JMP $"+str(self.info["set_entry"])]

		pointors = {}
		for k,v in self.code.items():
			pointors[k] = len(code)
			code = code + v
		
		for i in range(len(code)):
			for k,v in pointors.items():
				code[i] = code[i].replace("$"+k,str(v))
			if code[i] == "": code[i] = "NOP"
		
			

		for line in code:
			if "$" in line:
				raise ParserError("code","Pointor `"+str(line.split("$")[1].split(" ")[0])+"` is unknow.")
		

		size = self.info["size"]
		if size == None:
			size = max([ele.x for ele in argument_list],default=1)

		ram = []
		for cmd,val in self.ram:
			if cmd == "": continue
			if cmd == "arg":
				try:
					ram.append(argument_list[val])
				except IndexError:
					raise ParserError("ram","Index out of range in argument `"+str(val)+"`") 
			elif cmd == "arg_size":
				ram.append(argument_list[val].x * Matrix.IDENTITY(size))

		process = MatrixProcessor(code,ram,size,self.info["nb_reg"])
		
		try:
			while True:
				ret = process.iterate()
				if self.info["debug"]:
					print(ret)
		except ProcessError as psr:
			raise psr
		except ProcessEnd as psr:
			print("Algorithm executed sucessfully. End at "+str(psr.line))
			return psr.code
	


	def parse_ram_section(self,line):
		line[0] = line[0].lower()
		if len(line)<=1:
			raise ParserError("ram","Ram command `"+str(line[0])+"` have no arguments.")
		if line[0] == "arg":
			try:
				self.ram.append(("arg",int(line[1])))
			except ValueError:
				raise ParserError("ram","Ram command `"+str(line[0])+"` need a positive integer.")
		elif line[0] == "arg_size":
			try:
				self.ram.append(("arg_size",int(line[1])))
			except ValueError:
				raise ParserError("ram","Ram command `"+str(line[0])+"` need a positive integer.")
		else:
			raise ParserError("info","Info command `"+str(line[0])+"` unknown.")


	def parse_info_section(self,line):
		line[0] = line[0].lower()
		if len(line)<=1:
			raise ParserError("info","Info command `"+str(line[0])+"` have no arguments.")
		if line[0] == "set_entry":
			self.info["set_entry"] = line[1]
		elif line[0] == "debug":
			self.info["debug"] = "on"==line[1]
		elif line[0] == "nb_reg":
			try:
				self.info["nb_reg"] = int(line[1])
			except ValueError:
				raise ParserError("info","Info command `"+str(line[0])+"` need a positive integer.")

		elif line[0] == "nb_reg":
			try:
				self.info["nb_reg"] = int(line[1])
			except ValueError:
				raise ParserError("info","Info command `"+str(line[0])+"` need a positive integer.")
		elif line[0] == "size":
			if line[1] == "auto":
				self.info["size"] = None
			else:
				try:
					self.info["size"] = int(line[1])
				except ValueError:
					raise ParserError("info","Info command `"+str(line[0])+"` need a positive integer.")
		else:
			raise ParserError("info","Info command `"+str(line[0])+"` unknown.")



	def parse_code_section(self,line):
		if line[0][-1] == ":":
			self.active_code_sect = line[0][:-1]
			line = line[1:]
		self.code[self.active_code_sect] = self.code.get(self.active_code_sect,[]) + [" ".join(line)]

