#!/usr/bin/env python3
#Advent of Code Day 16 "Snailfish"
import sys
sys.path.append("..")

import submarine
sub = submarine.Submarine()

calc = sub.calculator

file = open('input.txt','r')
for line in file.readlines():
    calc.initialize(line)
    print(calc.tree)
    sys.exit(0)