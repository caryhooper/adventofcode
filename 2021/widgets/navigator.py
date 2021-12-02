class Navigator():
	def __init__(self):
		self.movements = list()
		pass

	def parse_movement_commands(self,text_list):
		#Input a text list of newline-separated commands and \
		#	output a list of movement units, tuples of the form
		#	(x-axis units, depth units) and save to self.movements
		for line in text_list:
			try:
				command,value = line.strip().split(' ')

				if command == "forward":
					self.movements.append((int(value),0))
				elif command == "down":
					self.movements.append((0,int(value)))
				elif command == "up":
					self.movements.append((0,int(value)*-1))
				else:
					#Debug
					print(f"Command did not match expected value: {line}.")
					pass
			except:
				pass
		
	def analyze_movement(self):
		#DEPRECATED - depth is dependent on aim
		#Input a list of movement tuples
		#Output the relative location in the format (x-axis units, depth units)
		x_units = 0
		depth = 0
		for x_mov,depth_move in self.movements:
			x_units += x_mov
			depth += depth_move
			# print(f"Forward {x_mov} ({x_units})- Depth {depth_move} ({depth})")

		print(f"The submarine has moved forward {x_units} units ")
		print(f"and plunged to a depth of {depth} units deep. ")
		print(f"(Product is {x_units *  depth})")
		return x_units, depth

	def analyze_movement_ng(self):
		#Input a list of movement tuples
		#Output the relative location in the format (x-axis units, depth units)
		x_units = 0
		depth = 0
		aim = 0
		for x_mov,aim_move in self.movements:
			aim += aim_move
			x_units += x_mov
			depth += x_mov * aim
			# print(f"Forward {x_mov} ({x_units})- Depth {depth_move} ({depth})")

		print(f"The submarine has moved forward {x_units} units ")
		print(f"and plunged to a depth of {depth} units deep. ")
		print(f"(Product is {x_units *  depth})")
		return x_units, depth