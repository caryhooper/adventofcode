#!/usr/bin/env python3
#Advent of Code Day 8
import sys
sys.path.append("..")

import submarine
sub = submarine.Submarine()

# Old #Part 1
# file = open('input.txt','r')
# sub.navigator.display.parse_diagnostic_input(file.readlines())
# count = sub.navigator.display.count_unique_output()
# print(f"There are {count} 1s, 4s, 7s, or 8s in the output.")

file = open('input.txt','r')
sub.navigator.display.parse_diagnostic_input(file.readlines())

# Part 1
count = sub.navigator.display.uniq_count
print(f"There are {count} 1s, 4s, 7s, or 8s in the output.")

#Part 2
total = 0
for i in sub.navigator.display.totals:
	total += i
print(f"The total output is {total}")


