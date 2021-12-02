#!/usr/bin/env python3
#Advent of Code Day 1
import sys
sys.path.append("..")#https://stackoverflow.com/questions/1054271/how-to-import-a-python-class-that-is-in-a-directory-above

import submarine
sub = submarine.Submarine()

#Part 1
file = open('input.txt','r')
depths_list = [ int(m.strip()) for m in file.readlines()]
sub.depthfinder.analyze_depths(depths_list)

#Part 2
rolling_3_average_depths_list = [ ((depths_list[i] + depths_list[i+1] + \
							depths_list[i+2])/3) for i in range(0,len(depths_list)-2)]
sub.depthfinder.analyze_depths(rolling_3_average_depths_list)
