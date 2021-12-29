class Lanternfish:
	def __init__(self,age=-1):
		if age == -1:	
			#Indicates brand new fish
			self.cycle_counter = 9
		else:
			self.cycle_counter = age

	def age(self):
		if self.cycle_counter == 0:
			self.cycle_counter = 7
		self.cycle_counter -= 1
