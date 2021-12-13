from os import closerange
import numpy,sys

class Navigator:
	def __init__(self):
		self.movements = list()
		self.display = SevenSegmentDisplay()
		self.low_points = list() #list with tuples as keys .. used in calculate_cave_risk
		self.basins = dict() #Keys are low_points, values are a set of basin points
		self.cave_risk_score = 0
		self.cave_grid_x_max = 0
		self.cave_grid_y_max = 0
		self.syntax_key = {"(": ")","[":"]","{":"}","<":">"}
		self.chunk_start = list(self.syntax_key.keys())
		self.chunk_end = list(self.syntax_key.values())
		self.first_error_chars = list()
		self.octopus_navigator = OctopusNavigator()
		self.cave_graph = CaveGraph()
		self.thermal_camera = ThermalCamera()

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

	def calculate_cave_risk(self,inlist):
		self.cave_grid_y_max = len(max(inlist, key=len))
		self.cave_grid_x_max = len(inlist)
		#Initialize numpy array for cave_grid
		list_of_lists = [[int(char) for char in line] for line in inlist]
		self.cave_grid = numpy.array(list_of_lists,numpy.int32)

		#Find all low points within the cave_grid and save to low_point dict.
		for x in range(0,self.cave_grid_x_max):
			for y in range(0,self.cave_grid_y_max):
				coord = (x,y)
				if self.is_low_point(x,y):
					self.low_points.append(coord)

		#Initialize basin data
		for low in self.low_points:
			self.basins[low] = set()
		#Find all basins
		self.find_basins()

		#Calculate Cave Risk Score
		for low in self.low_points:
			val = self.cave_grid[low]
			self.cave_risk_score += val + 1

	def get_basin_values(self):
		coords = list()
		for items in self.basins.values():
			[coords.append(coord) for coord in items]
		return coords

	def walk_cave_grid(self,coord):
		x,y = coord
		val= self.cave_grid[coord]
		neighbors = self.find_neighbors(x,y)
		least_neighbor_value = 9
		for neighbor in neighbors:
			#Find least adjacent value
			neighbor_val = self.cave_grid[neighbor]
			if neighbor_val < least_neighbor_value:
				least_neighbor_value = neighbor_val
				new_coords = neighbor
		
		#Walk downhill
		return new_coords

	def find_basins(self):
		#Iterate through all points and walk the grid to determine which basin it is in.
		for x in range(0,self.cave_grid_x_max):
			for y in range(0,self.cave_grid_y_max):
				coord = (x,y)
				#Continue to next value if it contains a 9 (no basin)
				if self.cave_grid[coord] == 9:
					continue
				#Check if coord already logged into a basin. 
				if coord in self.get_basin_values():
					continue
				
				#Otherwise, we need to find the low point sink. Using i,j for granular changes
				basin_coords = list()
				basin_coords.append(coord)
				walked_coord = coord
				while walked_coord not in self.low_points:
					walked_coord = self.walk_cave_grid(walked_coord)
					#Add to tracked set of coords in the same basin if not there already
					if walked_coord not in basin_coords:
						basin_coords.append(walked_coord)
				
				low_point = walked_coord
				#For each walked coord, add to the low_point basin.
				#basin values are sets so they won't duplicate
				[self.basins[low_point].add(coord) for coord in basin_coords]
				# print(f"Found basin for low point {low_point} of length {basin_coords}")





				# logged_points = set()
				# # print(f"Adding {coord} to logged_points")
				# logged_points.add(coord)
				# #While loop, stop when we reach a low point.
				# if x == 10 and y == 1:
				# 	print(f"Entering while loop for point {coord} with value {self.cave_grid[coord]}")
				# 	print(f"{coord} is low point? {self.is_low_point(x,y)}")



				# count = 0
				# # print(f"low_points = {self.low_points}")
				# while (i,j) not in self.low_points:
				# 	# print(f"next point {i},{j} with value {self.cave_grid[i,j]}")
				# 	neighbors = self.find_neighbors(i,j)
				# 	#Find the tuple coord with the lowest value
				# 	minimium_val = min([self.cave_grid[n] for n in neighbors])
				# 	# print(minimium_val)
				# 	new_coord = next(n for n in neighbors if self.cave_grid[n] == minimium_val)
				# 	# print(new_coord)
				# 	logged_points.add(new_coord)
				# 	i,j = new_coord
				# 	# print(f"new_coord is {new_coord}")
					# count += 1
					# if count == 5:
					# 	import sys
				# print(f"While loop ended")
				# print(f"Logged Points in basin: {logged_points}")
				# print(f"{self.cave_grid[0:15,0:15]}")
				# sys.exit(1)
				#If this breaks, new_coord is equal to a low point
				# if new_coord not in self.basins.keys():
				# 	# print(f"Error: {new_coord} should be a low point and already logged in {self.basins}")
				# 	# self.basins[new_coord] = logged_points
				# 	pass

				# else:
				# 	# print(f"Entry already in basin... adding logged_points: {logged_points}")
				# 	for point in logged_points:
				# 		self.basins[new_coord].add(point)
				# from pprint import pprint
				# pprint(f"Basins: {self.basins}")
				# if y == 10:
				# 	sys.exit()

	def find_neighbors(self,x,y):
		#Find all neighbors (set of tuples) adjacent to a x,y coord.
		#Note... no diagonals.  See OctopusNavigator for diagonals.
		neighbors = set()
		for i,j in [(0,1),(0,-1),(1,0),(-1,0)]:
			if x+i == self.cave_grid_x_max or x + i < 0:
				i = 0
			if y+j == self.cave_grid_y_max or y + j < 0:
				j = 0
			neighbors.add((x + i,y + j))
		neighbors.discard((x,y))
		return neighbors

	def is_low_point(self,x,y):
		neighbors = self.find_neighbors(x,y)
		#https://stackoverflow.com/questions/10666163/how-to-check-if-all-elements-of-a-list-match-a-condition
		if all(self.cave_grid[x,y] < self.cave_grid[i,j] for i,j in neighbors):
			print(f"Low point found at ({x},{y}) with value {self.cave_grid[x,y]}")
			print(f"\tNeighbors: {neighbors}")
			return True
		else:
			return False

	def extract_chunk(self,line):
		new_line = line
		progress = list()
		#Iterate through each character in the line
		for char in line:
			#If the char is a starting char, add to progress queue
			if char in self.chunk_start:
				progress.append(char)
			#If char is an ending char, check if correct one and remove from queue
			elif char in self.chunk_end:
				#Check for closing bracket is the correct one
				if char != self.syntax_key[progress[-1]]:
					print(f"Syntax Error! {char}")
					# print(f"Reached error with char {char}")
					self.first_error_chars.append(char)
					return
				else:
					progress.pop()
			else:
				# print(f"Error - illegal char found {char}")
				sys.exit(1)
		if len(progress) > 0:
			# print("Line is incomplete!")
			return progress

	def correct_syntax(self,diagnostic_input):
		completion_strings  = list()
		#progress works like a FIFO queue.
		#We'll .append() and .pop()
		for line in diagnostic_input:
			line = line.strip()
			#For each line, check if the chunk has errors and determine if incomplete
			progress = self.extract_chunk(line)
			if progress != None:
				#If not None, completion list is in progress
				progress.reverse()
				completion_string = [self.syntax_key[i] for i in progress ]
				completion_strings.append(completion_string)
		return completion_strings

class SevenSegmentDisplay:
	def __init__(self):
		#Dict matching the pattern {id : (first,output)}
		self.diagnostic_input = dict()
		#Just the raw totals of each line in a list
		self.totals = list()
		#May be more efficient to do with bitmasks and treat these as a 7bit code with numbers from 3 to 128.

		#   0:      1:      2:      3:      4:
		#  aaaa    ....    aaaa    aaaa    ....
		# b    c  .    c  .    c  .    c  b    c
		# b    c  .    c  .    c  .    c  b    c
		#  ....    ....    dddd    dddd    dddd
		# e    f  .    f  e    .  .    f  .    f
		# e    f  .    f  e    .  .    f  .    f
		#  gggg    ....    gggg    gggg    ....

		#   5:      6:      7:      8:      9:
		#  aaaa    aaaa    aaaa    aaaa    aaaa
		# b    .  b    .  .    c  b    c  b    c
		# b    .  b    .  .    c  b    c  b    c
		#  dddd    dddd    ....    dddd    dddd
		# .    f  e    f  .    f  e    f  .    f
		# .    f  e    f  .    f  e    f  .    f
		#  gggg    gggg    ....    gggg    gggg

		# self.key = {
		# 	0		:	'abcefg'	,	#len 6
		# 	1		:	'cf' 	,	#uniq len 2
		# 	2		:	'acdeg'	,	#len 5
		# 	3		:	'acdfg'	,	#len 5
		# 	4		:	'bcdf'	,	#uniq len 4
		# 	5		:	'abdfg'	,	#len 5
		# 	6		:	'abdefg'	,	#len 6
		# 	7		:	'acf' 	,	#uniq len 3
		# 	8		:	'abcdefg'	,	#uniq len 7
		# 	9		:	'abcdfg'		#len 6
		# }
		self.all_letters = 'abcdefg'
		self.uniq_count = 0

	def parse_diagnostic_input(self,input_lines):
		count = 0
		for line in input_lines:
			if line != '\n':
				diag,output = line.split('|')
				output = output.strip()
				diag_list = [i.strip() for i in diag.split(' ') if i != '']
				output_list = [i.strip() for i in output.split(' ') if i != '']
			self.diagnostic_input[count] = (diag_list,output_list)

			self.find_output_value(self.diagnostic_input[count])

			count +=1 

	def find_output_value(self,segment_tuple):

		#Helper functions to do operations on strings

		#Does a NOT operation on a string for self.all_letters
		def find_negative_letters(num_string):
			return ''.join([i for i in self.all_letters if i not in num_string])

		#Returns True iff string1 contains all letters in string2
		def contains(num_string1,num_string2):
			for i in num_string2:
				if i not in num_string1:
					return False
			return True

		#Removes a sorted string from a set
		def discard(this_set,item):
			item = "".join(sorted(item))
			return this_set.discard(item)

		#Program Logic:
		#So we can classify 1,4,7,8  Given these, what else can we classify?
		# 3 is the only len 5 that uses all the segments of 1
		# 9 is the only len 6 that uses all segments of 4

		# 2 is the only len 5 remaining that uses the only letter missing from 9
		# 5 is the other len 5 and is also missing this same letter missing from 9
		# 0 contains both letters missing from 5
		# 6 contains one letter missing from 5

		segments,output = segment_tuple
		#combine both lists
		segments.extend(output)

		#Sort in alphabetical order and create a set of length 10
		remaining_segments = set(["".join(sorted(i)) for i in segments])

		#Each of these has a uniqeu length
		one 	= 	list(filter(lambda x : len(x) == 2, remaining_segments))[0]
		four 	= 	list(filter(lambda x : len(x) == 4, remaining_segments))[0]
		seven 	=	list(filter(lambda x : len(x) == 3, remaining_segments))[0]
		eight 	= 	list(filter(lambda x : len(x) == 7, remaining_segments))[0]
		#Discard from remaining_segments
		for num in [one,four,seven,eight]:
			discard(remaining_segments,num)

		#Find 3 - the only len 5 that uses all the segments of 1
		for num in remaining_segments:
			if len(num) == 5:
				if contains(num,one):
					three = num
		discard(remaining_segments,three)
		
		#Find 9 - the only len 6 that uses all segments of 4
		for num in remaining_segments:
			if len(num) == 6:
				if contains(num,four):
					nine = num
		discard(remaining_segments,nine)

		#Find 2 - the only len 5 remaining that uses the only letter missing from 9
		for num in remaining_segments:
			if len(num) == 5:
				nine_negative = find_negative_letters(nine)
				if contains(num,nine_negative):
					two = num
		discard(remaining_segments,two)

		#Find 5 - the other len 5
		five 	= 	list(filter(lambda x : len(x) == 5, remaining_segments))[0]
		discard(remaining_segments,five)
		
		#Find 0 -  contains both letters missing from 5
		zero = list(filter(lambda x : contains(x,find_negative_letters(five)), remaining_segments))[0]
		discard(remaining_segments,zero)

		#Six is last one remaining
		six = remaining_segments[0]
		discard(remaining_segments,six)

		if len(remaining_segments) != 0:
			print(f"Error:  remaining_segments has {len(remaining_segments)} items remaining: {remaining_segments}")
			import sys
			sys.exit(1)

		#Construct local key to convert string to digits {'abcde':0,'ef':1,...}
		key = dict()
		count = 0
		for i in [zero,one,two,three,four,five,six,seven,eight,nine]:
			key[i] = count
			count += 1

		#Extract all digits of output list, sorted in alphabetical order and convert to int via key
		d1,d2,d3,d4 = [ key["".join(sorted(i))] for i in output]
		#manually construct total
		final_number = 1000*d1 + 100*d2 + 10*d3 + d4
		
		#Send total to count_unique_output to satisfy Part 1
		self.count_unique_output(final_number)
		#Save all totals to the totals list for access later
		self.totals.append(final_number)

	def count_unique_output(self,total):
		count = 0
		uniq_len = ["1","4","7","8"]
		#Count all unique length digits in the total.
		for l in uniq_len:
			if l in str(total):
				count += 1
		self.uniq_count += count

class OctopusNavigator:
	#Octopus Navigator class takes in a 10x10 grid of octopus power levels
	def __init__(self):
		#Counts the number of flashes total
		self.flash_count = 0
		#Counts the number of steps taken
		self.steps = 0
		#Records the first time all octopi flash at the same time.  
		self.first_simultaenous_flash_step = 0

	def initialize_octopi(self, seed_strings):
		#Seed self.grid with a numpy array consisting of all the initial seeds
		all_rows = list()
		for line in seed_strings:
			row = [o for o in line.strip()]
			all_rows.append(row)
		#Initialize seed strings into numpy array
		self.grid = numpy.array(all_rows,numpy.int32)
		#Keep track of x_max and y_max for looping
		self.x_max = len(self.grid[0])
		self.y_max = len(self.grid)

	def move_one_step(self):
		print(f"Beginning Step {self.steps + 1}")
		#First, add 1 to each energy level (all values)
		self.grid += 1
		flashed_octopi = set() #initialize structure to track octopi that have already flashed this round
		#This while loop is the core of the logic.  While an octopus is > 9 and hasn't flashed already, do flashing ops
		while self.has_greater_than_nine(flashed_octopi):
			flashed_octopi = self.do_flashes(flashed_octopi)
		#Zero records greater than 9 that haven't already flashed.
		#Zero out each of these grid squares.
		for x,y in flashed_octopi:
			self.grid[x,y] = 0
		#Increment Step
		self.steps += 1
		print(f"Step {self.steps} complete.")

		#Check if self.grid is all zeros
		zeros = numpy.array([[[0] * self.x_max] * self.y_max],numpy.int32)
		if numpy.array_equiv(zeros,self.grid):
			self.first_simultaenous_flash_step = self.steps
			print(f"All octopi flashed at step {self.steps}!")

	def do_flashes(self, flashed_octopi):
		#Iterate through self.grid array and increment neighbors if an octopus is > 9 power
		for x in range(0,self.x_max):
			for y in range(0,self.y_max):
				coord = (x,y)
				#Flash if not flashed already
				if self.grid[x,y] > 9 and coord not in flashed_octopi:
					#Find a set of neighboring tuples
					neighbors = self.find_adjacent_coords(x,y)
					#Increment neighbors
					for i,j in neighbors:
						self.grid[i,j] += 1
						#Increment total flash count
					self.flash_count += 1	
					#Add to flashed set / return flashed set
					flashed_octopi.add(coord)
		return flashed_octopi

	def has_greater_than_nine(self, flashed_octopi):
		#function iterates and determines if there exists an octopus that
		#    is greater than 9 power that hasn't already flashed
		for x in range(0,self.x_max):
			for y in range(0,self.y_max):
				if self.grid[x,y] > 9 and (x,y) not in flashed_octopi:
					return True
		return False


	def	find_adjacent_coords(self,x,y):
		#This is a really nice way to determine neighbors of a coordinate.  This one return a
		#    set of tuples neighboring a point x,y
		neighbors = set()
		for i,j in [(0,1),(0,-1),(1,0),(-1,0),(1,1),(-1,-1),(1,-1),(-1,1)]:
			if x+i == self.x_max or x + i < 0:
				i = 0
			if y+j == self.y_max or y + j < 0:
				j = 0
			neighbors.add((x + i,y + j))
		neighbors.discard((x,y))
		return neighbors

class CaveGraph:
	def __init__(self):
		self.graph = dict() #Adjacency graph is a dict which has keys of start points and
		#	values are a list of end points.  Basically a bunch of nodes and edges
		#initialize counters and constants.   Don't need self.big_caves, but may need later
		self.small_caves = list()
		self.big_caves = list()
		self.path_count = 0

	def load_graph_data(self,lines):
		#Input a list of newline-separated edgest
		#Load vectors into adjacency graph
		for line in lines:
			line = line.strip()
			start,end = line.split('-')
			#If not already in the graph, add as a key and initialize to set()
			if start not in self.graph.keys():
				self.graph[start] = set()
			if end not in self.graph.keys():
				self.graph[end] = set()
			
			#These paths should be bi-directional
			self.graph[start].add(end)
			self.graph[end].add(start)

		for i in self.graph.keys():
			#Sorting each so they're in the same order when debugging
			self.graph[i] = list(self.graph[i])
			self.graph[i].sort()
		
		#Add all the things to big_caves and small_caves attributes
		[self.big_caves.append(i) for i in self.graph.keys() if i.isupper()]
		[self.small_caves.append(i) for i in self.graph.keys() if i.islower()]
		
	def walk_next_path(self,current_path = ["start"],can_visit_a_small_cave_twice=False):
		#This function does the heavy lifting. 
		#Recursive function that walks the graph, given a "current_path", determine
		#   (1) Has the path ended?
		#   (2) Have we broken any of the rules?
		#   (3) Can we continue adding to the path?

		#Track the last node to determine where we can go
		last_node = current_path[-1]
		#loop through all nodes that we can go
		for next_node in self.graph[last_node]:
			#Add the potential node to the path
			current_path.append(next_node)
			if next_node == "end":
				#We've reached the exit.  Count as valid path
				self.path_count += 1
				current_path.pop()
				continue
			elif next_node == "start":
				#We're back at the start.  This is illegal.  Pop/continue
				current_path.pop()
				continue
			elif next_node in self.small_caves and next_node in current_path[:-1]:
				#We reached a small cave where we've been before.
				if can_visit_a_small_cave_twice:
					#This is true when its not necessarily illegal to have been to a small cave twice
					#This function returns all repeat small caves in the path ["c","c"]
					twice_caves = self.caves_already_visited_twice(current_path)
					#If more than 2 (3 or 4) we have three small or two and two small (both illegal)
					if len(twice_caves) > 2:
						#Pop/Continue
						current_path.pop()
						continue
					#If not more than one we can add to the path.  
					else:
						#Walk the path and keep the rules the same.
						self.walk_next_path(current_path, can_visit_a_small_cave_twice=can_visit_a_small_cave_twice)
						current_path.pop()
						continue
				else:
					#Case for when we've reached a dead-end small cave (already visited)
					current_path.pop()
					continue
			else:
				# Here, we can add to the current path and recurse further, keeping the rules the same
				self.walk_next_path(current_path, can_visit_a_small_cave_twice=can_visit_a_small_cave_twice)
				current_path.pop()
				continue
			
	def caves_already_visited_twice(self,current_path):
		#Helper stub to return a list of repeated small caves within an array
		repeated_small_caves = list()
		for cave in current_path:
			if cave in self.small_caves:
				#Check if cave is small
				#Only append to repeated_small_caves if the count is greater than 1
				count = sum(1 for i in current_path if i == cave)
				if count > 1:
					repeated_small_caves.append(cave)
		return repeated_small_caves

class ThermalCamera:
	def __init__(self):
		self.dots = list()
		self.folds = list()

	def activate(self,lines):
		#Initialize Data Structures and do folds
		self.parse_manual(lines)
		#Finds the max fold value and doubles and adds 1 for max coords
		self.x_max = max([j for i,j in self.folds if i == 'x']) * 2 + 1
		self.y_max = max([j for i,j in self.folds if i == 'y']) * 2 + 1
		#Initialize numpy array with all 0s
		self.grid = numpy.array([[0] *  self.y_max] * self.x_max,numpy.int32)
		#For each dot, make the (x,y) value equal to 1
		for dot in self.dots:
			self.grid[dot] = 1

	def parse_manual(self,lines):
		#Parse through each line
		for line in lines:
			line = line.strip()
			if "fold along" in line:
				#Track folds as a tuple ("x",12) in self.folds
				axis,fold_val = line.split(" ")[2].split('=')
				self.folds.append((axis,int(fold_val)))
			elif ',' in line:
				#Track dots as an (x,y) tuple in self.dots
				x,y = line.split(',')
				self.dots.append((int(x),int(y)))
			else:
				#Newline or EOF
				pass

	def do_folds(self, fold_count=None):
		#Helper function to determine which fold.  
		for axis,fold_val in self.folds:
			# print(f"Folding across the {axis}={fold_val} axis")
			# print(f"Currently the grid is {self.grid.shape} with x_max of {self.x_max} and y_max of {self.y_max}")
			#Fold over x=4 means vertical fold (fold left)
			if axis == "x":
				temp_grid = self.fold_left(fold_val)
			#Fold over y=4 means horiz fold (fold up)
			elif axis == "y":
				temp_grid = self.fold_up(fold_val)
			#Case not reached during testing
			else:
				print(f"Error: unexpected value for {axis} in folds: {folds}")
				sys.exit(1)
			
			#Assign temp_grid to self.grid and calculate x_max and y_max
			self.grid = temp_grid
			self.x_max,self.y_max = self.grid.shape

			#logic to stop folding after X folds (Part 1)
			if fold_count:
				fold_count -= 1
				if fold_count == 0:
					break
			
			

	def fold_up(self, axis):
		#Folding up means y_max gets smaller and x_max stays the same
		#Create a temp_grid object to store results of fold (temp_grid.shape <= self.grid.shape)
		temp_grid = self.grid[:,0:axis]	
		#Iterate through all x,y in self.grid
		for x in range(0,self.x_max):
			for y in range(0,self.y_max):
				#If coord is below fold
				if y > axis:
					#take the difference between the max - 1 and y
					new_y = self.y_max - y -1
					if new_y < 0:
						#Error handling
						continue
					else:
						#OR the current position and the new position.  If either is 1,
						#    temp_grid[x,new_y] will also be one
						temp_grid[x,new_y] = (self.grid[x,y] or self.grid[x,new_y])
				elif y == axis:
					#Skip all values for y that fall on the fold
					continue
				else:
					#Otherwise, just move the val from self.grid to temp_grid
					temp_grid[x,y] = self.grid[x,y]
					#temp_grid[x,y] = (self.grid[x,y] or temp_grid[x,y])

		return temp_grid

	def fold_left(self, axis):
		#Folding left means x_max gets smaller and y_max stays the same
		#Create a temp_grid object to store results of fold (temp_grid.shape <= self.grid.shape)
		temp_grid = self.grid[0:axis,:]	
		#Iterate through all x,y in self.grid
		for x in range(0,self.x_max):
			for y in range(0,self.y_max):
				#If coord is right of fold
				if x > axis:
					#New x is the difference between total size minus one and x
					new_x = self.x_max - x -1
					if new_x < 0:
						#Error handling
						continue
					else:						
						#OR the current position and the new position.  If either is 1,
						#    temp_grid[new_x,y] will also be one
						temp_grid[new_x,y] = (self.grid[x,y] or self.grid[new_x,y])
				elif x == axis:
					#Skip all values for y that fall on the fold
					continue
				else:
					#Otherwise, just move the val from self.grid to temp_grid
					temp_grid[x,y] = self.grid[x,y]
		return temp_grid

	def count_dots(self):
		return self.grid.sum()

	def print(self):
		#Iterate through all coords (x,y) in self.grid
		#Makes final solution more readable
		for y in range(0,self.y_max):
			for x in range(0,self.x_max):
				#If val is 1, print in black on black
				if self.grid[x,y] == 1:
					print("\033[0;30;40m ",end='')
				#Else val is 0, print in white on white
				else:
					print("\033[0;37;47m ",end='')
			print("\033[0;30;40m\n",end='')
