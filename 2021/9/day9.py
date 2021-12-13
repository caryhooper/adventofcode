#!/usr/bin/env python3
#Advent of Code Day 9 "Smoke Basin"
import sys
sys.path.append("..")

import submarine
sub = submarine.Submarine()

#Part 1

#Trying out the map() method per Ryan to remove newlines from each entry
file = open('input.txt','r')
# file = open('input_test.txt','r')
lines_nonewline = list(map(str.strip,file.readlines()))
sub.navigator.calculate_cave_risk(lines_nonewline)
risk = sub.navigator.cave_risk_score
print(f"The cave risk score is {risk}")

#Part 2
#1304576 too high
#846846 too low
#874000 too low
#1280496 is just right
basin_sets = list(sub.navigator.basins.values())
product = 1
# for i in range(0,3):
# 	# largest_basin = max(basin_sets, key=len)
# 	# print(f"{len(largest_basin)}")
# 	# product *= len(largest_basin)
# 	# basin_sets.remove(largest_basin)

largest_three = sorted(basin_sets,key=len,reverse=True)[0:3]
for i in largest_three:
	print(i)
	print(len(i))
	product *= len(i)

# for sink in sub.navigator.basins:
# 	print(f"Sink: {sink} || Basin : {sub.navigator.basins[sink]}")

# for i in sorted(sub.,key=len)[3:]:
# 	print(len(i))
# 	print(i)

print(f"The product of the three largest basins is: {product}")

# print(sub.navigator.low_points)
# for x,low_val in sub.navigator.low_points:
# 	print(f"Low point {x} of value {low_val}")