#!/usr/bin/env python3
#Advent of Code Day 15 "Chiton"
import sys
sys.path.append("..")

import submarine
sub = submarine.Submarine()

file = open("input.txt","r")
sub.navigator.grid.initialize(file.readlines())
sub.navigator.grid.find_shortest_path()

end = sub.navigator.grid.end_node
sp = end.distance
print(f"The shortest path is {sp} to {end}")
file.close()

# file = open("input.txt","r")
# sub.navigator.grid.load_chitons(file.readlines(), full_map=True)
# sub.navigator.grid.find_shortest_path()

# end = sub.navigator.grid.end_node
# sp = end.distance
# print(f"The shortest path is {sp} to {end}")