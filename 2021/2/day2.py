#!/usr/bin/env python3
#Advent of Code Day 2
import sys
sys.path.append("..")

import submarine
sub = submarine.Submarine()

file = open('input.txt','r')

#Part 1
sub.navigator.parse_movement_commands(file.readlines())
sub.navigator.analyze_movement()

#Part 2
sub.navigator.analyze_movement_ng()