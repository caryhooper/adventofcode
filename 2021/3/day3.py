#!/usr/bin/env python3
#Advent of Code Day 2
import sys
sys.path.append("..")

import submarine
sub = submarine.Submarine()

file = open('input.txt','r')

#Part 1
sub.diagnostics.parse_diagnostics(file.readlines())
g = sub.diagnostics.gamma_rate
e = sub.diagnostics.epsilon_rate
print(f"The power consumption gamma and epsilon rates are: {g} and {e}")
print(f"The power consumption is {sub.diagnostics.power_consumption}")

#Part 2
print(f"The life support rating is {sub.diagnostics.life_support_rating}\n\t(oxygen rating of {sub.diagnostics.oxygen_rating} and co2 rating of {sub.diagnostics.co2_rating})")