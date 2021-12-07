#!/usr/bin/env python3
#Advent of Code Day 7
import math, statistics

#Part 1
crab_positions = list()
[crab_positions.append(int(coord.strip())) for coord in open('input.txt','r').read().strip().split(',')]

#Calculate Median
med = math.floor(statistics.median(crab_positions))
count = 0
#Calculate fuel needed (diff between median and each crab)
for digit in crab_positions:
	count += abs(digit - med)
print(f"Crab movement to {med} will take {count} fuel.")

#Part 2
#Minimize the following function.  choose an x such that f(x) equals the SUM( crab_fuel(i - x) )
#dist#1 2 3  4  5  6
#fuel#1 3 6 10 15 21
#fuel = d(d+1) / 2
#The function will be minimized at the average instead of the median.

mean = math.floor(statistics.mean(crab_positions))
count = 0
#Calculate fuel needed
for digit in crab_positions:
	dist = abs(digit - mean)
	count += (dist * (dist + 1))/2 #Always an int
print(f"Crab movement to {mean} will take {count} fuel.")
