#Class for Day 1 to keep track of whether (and how much) 
#the depth is increasing and decreasing.

class Depthfinder:
	def __init__(self):
		pass

	def analyze_depths(self,depth_array):
		#Accepts a depth_array of integers and outputs the number of times
		#that the depths increase and decrease. Day1
		depths_diff = list()
		prev = depth_array[0]
		increase_count = 0

		for curr in depth_array[1:]:
			delta = curr - prev
			depths_diff.append(delta)
			prev = curr
		for diff in depths_diff:
			if diff > 0:
				increase_count += 1

		print(f"The depth increased {increase_count} times.")
