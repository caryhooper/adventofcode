#!/usr/bin/env python3
#Advent of Code Day 13 "Transparent Oragami"
import sys
sys.path.append("..")

import submarine
sub = submarine.Submarine()

#Part 1
thermal = sub.navigator.thermal_camera
file = open('input.txt','r')
#Activate camera by feeding input from manual
thermal.activate(file.readlines())

#Fold once
thermal.do_folds(fold_count=1)
print(f"The number of dots after the first fold is {thermal.count_dots()}")

#Part 2
#Fold until complete
thermal.do_folds()
#Print result after folds
thermal.print()