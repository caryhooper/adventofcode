#!/usr/bin/env python3
#Advent of Code Day 5
import sys
sys.path.append("..")

import submarine
sub = submarine.Submarine()

file = open('input.txt','r')

#Part 1
# sub.diagnostics.map_vents(file.readlines(), square_only=True)
# overlaps = sub.diagnostics.vent_mapper.calc_overlapping_vents()
# print(f"There are {overlaps} points where vents overlap.")


#Part 2
sub.diagnostics.map_vents(file.readlines())
overlaps = sub.diagnostics.vent_mapper.calc_overlapping_vents()
print(f"There are {overlaps} points where vents overlap.")