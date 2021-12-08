class Navigator:
	def __init__(self):
		self.movements = list()
		self.display = SevenSegmentDisplay()
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
