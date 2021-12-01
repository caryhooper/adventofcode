#!/usr/bin/env python3
#Advent of Code Day 1

file = open('input.txt','r')

depths = list()
depths_diff = list()
depths_diff.append("N/A")
prev = int(file.readline().strip())

# measurements = file.readlines()[1:]
#Added for Part 2
measurements_int = [ int(m.strip()) for m in file.readlines()]
m = measurements_int
measurements_3way_int = [ (measurements_int[i] + measurements_int[i+1] + \
							measurements_int[i+2]) for i in range(0,len(m)-2)]

measurements = measurements_3way_int
print(measurements[:20])

for line in measurements:
	current = line
	delta = current - prev
	depths_diff.append(delta)
	prev = current

increases_count = 0
for diff in depths_diff:
	try:
		diff = int(diff)
		if diff > 0:
			increases_count += 1
	except:
		pass

# print(f"The depth increased {increases_count} times.")

print(f"The rolling-3 count increased {increases_count} times.")