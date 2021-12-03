import sys
import operator

class Diagnostics:
	#Note: parse_diagnostics(input_file) must be called to diagnose the sub
	def __init__(self):

		self.max_bits = 16
		self.gamma_rate = 0
		self.epsilon_rate = 0
		self.power_consumption = 0
		self.diagnostic_list = list()
		self.life_support_rating = 0
		self.oxygen_rating = 0
		self.co2_rating = 0
		pass

	def bitstring_to_int(self,bitstring):
		#input a string of 1s and 0s, output an int
		return int(bitstring,2)

	def int_to_bitstring(self,my_int):
		#input an int and output a string of 1s and 0s
		return "{0:b}".format(my_int)

	def clear_item_from_list(self,item,the_list):
		#Use "filter" to recursively remove items from a list, return the list
		return list(filter((item).__ne__,the_list))

	def parse_diagnostic_input(self, diag_list):
		#Called from parse_diagnostics if not called already.
		#Input a list of diagnostic binary strings and save to object attribute as strings
		for entry in diag_list:
			#Skip newlines or blank lines
			if entry != "\n" and entry != "":
				entry = entry.strip()
				self.diagnostic_list.append(entry)

	def calc_power_consumption(self):
		#Parse through diagnostic input, convert to ints, then count each
		#bit occurence.  Depending on the bit occurrences, calculate power consumption.
		#Initialize arrays to count bit occurrences
		zero_count = [0] * self.max_bits
		one_count = [0] * self.max_bits
		
		#Iterate through diagnostics
		for entry in self.diagnostic_list:
			#Convert string to integer using builtin int(x,2)
			b = self.bitstring_to_int(entry)
			#Iterate through bits
			for bit in range(0,len(entry)):
				#AND each bit with the corresponding mask and add to bit counter
				ans = b & 2**bit
				if b & 2**bit == 0:
					zero_count[bit] += 1
				else:
					one_count[bit] += 1

		#Next, we translate these counts to the power bit strings
		#Initialize gamma and epsilon bitstrings
		gamma_rate_bitstring = ""
		epsilon_rate_bitstring = ""
		#Iterate through each count in the bit counters
		for bit in range(0,self.max_bits):
			#Build bit strings depending on counts
			if zero_count[bit] == 0 and one_count[bit] == 0:
				#Case for cieling being met
				gamma_rate_bitstring = "0" + gamma_rate_bitstring
				epsilon_rate_bitstring = "0" + epsilon_rate_bitstring
			elif zero_count[bit] >= one_count[bit]:
				gamma_rate_bitstring = "0" + gamma_rate_bitstring
				epsilon_rate_bitstring = "1" + epsilon_rate_bitstring
			else:
				gamma_rate_bitstring = "1" + gamma_rate_bitstring
				epsilon_rate_bitstring = "0" + epsilon_rate_bitstring

		# print(f"gamma_rate_bitstring   = \"{gamma_rate_bitstring}\"")
		# print(f"epsilon_rate_bitstring = \"{epsilon_rate_bitstring}\"")
		
		#Convert bit strings to ints and save as attributes
		self.gamma_rate = self.bitstring_to_int(gamma_rate_bitstring)
		self.epsilon_rate = self.bitstring_to_int(epsilon_rate_bitstring)
		#Finally, calculate total power_consumption and save.
		self.power_consumption = self.gamma_rate * self.epsilon_rate

	def parse_diagnostics(self, diag_list):
		#If list hasn't been ingested, ingest it now.  This should be the entry to the 
		if len(self.diagnostic_list) < 1:
			self.parse_diagnostic_input(diag_list)

		#Caclulate power consumption and gas ratings
		self.calc_power_consumption()
		self.calc_gas_ratings()

	def calc_gas_ratings(self):
		#Oxy rating (left to right): If more 1s than 0s, eliminate all 0s
		#Next bit... more 0s, eliminate all 1s (if 1s = 0s, keep 1s)
		self.calc_gas_rating("oxygen")

		#Co2 rating (left to right): Fewer 0s so remove all 1s
		#Next bit... fewer 1s so remove 0s (if 1s = 0s, keep 0s)
		self.calc_gas_rating("co2")
		self.life_support_rating = self.oxygen_rating * self.co2_rating

	def calc_gas_rating(self,mode):
		if mode == "oxygen":
			op = operator.ge
		elif mode == "co2":
			op = operator.lt
		else:
			print(f"Error: mode must be oxygen or co2: {mode}")
			sys.exit(1)

		#From the input data, calculate the length of the longest binary string
		longest_diag = max(self.diagnostic_list, key=len)
		highest_order_bit = len(longest_diag) - 1

		#convert all values to int and store in gas_list
		gas_list = [self.bitstring_to_int(i) for i in self.diagnostic_list]

		#Loop through until gas_list has one entry. 
		while len(gas_list) > 1 and highest_order_bit >= 0:
			# If highest_order_bit < 0, an error occurred and we were unable to eliminate enough
			ones_count = zeros_count = 0
			#Count all the 1s and 0s, starting with the highest order bit
			for entry in gas_list:
				if entry & 2**highest_order_bit == 0:
					#Indicates a 0
					zeros_count += 1
				else:
					#Indicates a 1
					ones_count += 1
			#Remove entries from gas_list.  op() is from the operator library, which depends on mode
			#For oxygen, the comparison is >= and for co2 it is <.
			if op(ones_count,zeros_count):
				for entry in gas_list:
					if (entry & 2**highest_order_bit > 0):
						gas_list = self.clear_item_from_list(entry,gas_list)
			else:
				for entry in gas_list:
					if (entry & 2**highest_order_bit == 0):
						gas_list = self.clear_item_from_list(entry,gas_list)
			#Decrement highest_order_bit to examine the next bit to the right
			highest_order_bit -= 1

		#Error handling...
		if highest_order_bit == 0 and len(gas_list) > 1:
			print(f"Error... length of gas_list is {len(gas_list)}.")
			sys.exit()

		#Assign result to attribute.  We've already handled errata in the "mode" param.
		if mode =="oxygen":
			self.oxygen_rating = gas_list[0]
		else:
			self.co2_rating = gas_list[0]
