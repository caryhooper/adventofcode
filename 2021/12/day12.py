#!/usr/bin/env python3
#Advent of Code Day 12 "Passage Pathing"
#Here, we create a graph of nodes/edges in a dictionary 
#And use a recursive function in the CaveGraph class to walk all possible paths
import sys
sys.path.append("..")

import submarine
sub = submarine.Submarine()

file = open('input.txt','r')
#Initialize Cave Graph object
cg = sub.navigator.cave_graph
#Load the files
cg.load_graph_data(file.readlines())

#Walk the cave where no small cave repeats allowed
cg.walk_next_path()
print(f"The total number of paths are: {cg.path_count}")

#Part 2
#Walk the cave where you can visit one small cave twice.   
cg.path_count = 0
cg.walk_next_path(can_visit_a_small_cave_twice=True)
print(f"The total number of paths are: {cg.path_count}")