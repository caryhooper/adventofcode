#!/usr/bin/env python3
#Advent of Code Day 16 "Packet Decoder"
import sys
sys.path.append("..")

import submarine, operator
sub = submarine.Submarine()

def bin2int(bitstring):
	#input a string of 1s and 0s, output an int
	return int(bitstring,2)

class Decoder:
	def __init__(self,input_string):
		self.bitstream = self.convert_to_bitstream(input_string)
		self.bit_counter = 0
		pass

	def parse_packets(self):
		self.packet = Packet(self.bitstream)
		# self.packet.parse()
		return self.packet
	def convert_to_bitstream(self,input_string):
		s = ""
		for c in input_string:
			s += self.hex2bin(c)
		return s

	def hex2bin(self,my_hex):
		#input a hex string and output a string of 1s and 0s
		num = int(my_hex,base=16)
		binary =  "{0:b}".format(num)
		pad = 4-len(binary)
		# print(f"my_hex {my_hex} becomes {binary}")
		return '0'*pad + binary

class Packet:
	def __init__(self,bitstring):
		self.bitstring = bitstring
		self.version = 0
		self.type_id = 0
		self.is_literal = True
		self.literal = ''
		self.packets = list()
		self.literals = list()
		self.literal_value = 0
		self.payload_length = 0
		self.payload_quant = 0
		self.bit_count = 0
		self.parse()
		self.version_sum = 0
		self.total_value = 0

	def parse(self):
		#Parse Version
		version = self.bitstring[0:3]
		self.version = self.bin2int(version)
		#Parse Type ID
		typeid = self.bitstring[3:6]
		self.type_id = self.bin2int(typeid)
		self.bit_count += 6
		self.bitstring = self.bitstring[6:]
		if self.type_id == 4:
			self.is_literal = True
		else:
			self.is_literal = False

		if self.is_literal:
			# remaining_size = len(self.bitstring[6:])
			# if remaining_size % 5 != 0:
			# 	print(f"Error with packet literal.  {remaining_size} bits remaining.")

			
			#Process rest of five-bit numbers within.
			first_bit = '1'
			
			while first_bit == '1':
				chunk = self.bitstring[0:5]
				first_bit = chunk[0]
				self.literals.append(chunk[1:])
				self.bitstring = self.bitstring[5:]
				self.bit_count += 5
			self.literal_value = self.bin2int("".join(self.literals))
		else:
			#Type is operator packet... this is where the fun begins. 
			lengthtypeid = self.bitstring[0]
			self.bitstring = self.bitstring[1:]
			self.bit_count += 1 
			if lengthtypeid == '1':
				#Represents number of sub-packets
				#11 bits representing number of sub-packets
				lengthbits = self.bitstring[0:11]
				self.payload_quant = self.bin2int(lengthbits)
				self.bitstring = self.bitstring[11:]
				self.bit_count += 11
				while self.payload_quant > 0:
					p = Packet(self.bitstring)
					self.bit_count += p.bit_count
					self.packets.append(p)
					self.bitstring = self.bitstring[p.bit_count:]
					self.payload_quant -= 1
			else:
				#Represents total length in bits
				# 15 #bits representing the total bit length of payload
				lengthbits = self.bitstring[0:15]
				self.payload_length = self.bin2int(lengthbits)
				self.bitstring = self.bitstring[15:]
				self.bit_count += 15

				while self.payload_length >= 11: #11 is smallest packet (literal with one value)
					p = Packet(self.bitstring)
					self.bit_count += p.bit_count
					self.packets.append(p)
					self.bitstring = self.bitstring[p.bit_count:]
					self.payload_length -= p.bit_count
					
					if "".join(set(self.bitstring)) == '0':
						remaining_zeros = len(self.bitstring)
						self.payload_length -= remaining_zeros
						self.bit_count += remaining_zeros
						self.bitstring = self.bitstring[remaining_zeros:]

	def bin2int(self,bitstring):
		#input a string of 1s and 0s, output an int
		return int(bitstring,2)
	
	def find_version_sum(self):
		self.version_sum += self.version
		for p in self.packets:
			vs = p.find_version_sum()
			self.version_sum += vs
		return self.version_sum

	def calculate_value(self):
		self.total_value = 0
		if not self.is_literal:
			#Recurse if there is an operator packet
			# for p in self.packets:
			if self.type_id == 0:
				op = operator.add
			elif self.type_id == 1:
				op = operator.mul
			elif self.type_id == 2:
				#minimum
				pass
			elif self.type_id == 3:
				#maximum
				pass
			elif self.type_id == 4:
				#Packet Literal
				self.total_value = p.literal_value
			elif self.type_id == 5:
				op = operator.gt
			elif self.type_id == 6:
				op = operator.lt
			else:
				#Type ID 7
				op = operator.eq

			if self.type_id in [0,1]:
				if self.type_id == 1:
					self.total_value += 1
				for p in self.packets:
					p.calculate_value()
					val = p.total_value
					self.total_value = op(self.total_value,val)
			if self.type_id in [2,3]:
				all_values = list()
				for p in self.packets:
					p.calculate_value()
					val = p.total_value
					all_values.append(val)
				if self.type_id == 3:
					self.total_value = max(all_values)
				else:
					self.total_value = min(all_values)
				
			if self.type_id in [5,6,7]:
				p1,p2 = self.packets
				p1.calculate_value()
				p2.calculate_value()
				result = op(p1.total_value,p2.total_value)
				if result:
					self.total_value = 1
				else:
					self.total_value = 0
		else:
			self.total_value =  self.literal_value

#Part 1
file = open('input.txt','r')
input_string = file.read().strip()
d = Decoder(input_string)
p = d.parse_packets()

print(f"The remaining bitstream is: {p.bitstring}")

print(f"The version sum for all packets is {p.find_version_sum()}")

#Part 2
p.calculate_value()
print(f"The total value of the packets is {p.total_value}")