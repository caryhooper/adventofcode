#!/usr/bin/env python3
#Advent of Code Day 16 "Packet Decoder"
import sys
sys.path.append("..")

import submarine
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
		self.bit_count = 3
		self.parse()

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
			self.is_literal == True
		else:
			self.is_literal == False

		if self.is_literal:
			remaining_size = len(self.bitstring[6:])
			if remaining_size % 5 != 0:
				print(f"Error with packet literal.  {remaining_size} bits remaining.")

			
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
			if lengthtypeid == 1:
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
					self.bitstring = self.bitstring[p.bit_count]
					self.payload_length -= p.bit_count

	def bin2int(self,bitstring):
		#input a string of 1s and 0s, output an int
		return int(bitstring,2)

file = open('input_op1.txt','r')
input_string = file.read().strip()
d = Decoder(input_string)
p = d.parse_packets()

print(f"The remaining bitstream is: {p.bitstring}")