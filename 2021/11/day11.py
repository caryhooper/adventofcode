#!/usr/bin/env python3
#Advent of Code Day 11 "Dumbo Octopus"
import sys
sys.path.append("..")

import submarine
sub = submarine.Submarine()

#Part1
file = open("input.txt","r")
nav = sub.navigator.octopus_navigator
nav.initialize_octopi(file.readlines())
file.close()

while nav.steps != 100:
    nav.move_one_step()
    print(nav.grid)

print(f"The number of flashes was {nav.flash_count}")

#Part 2

while nav.first_simultaenous_flash_step == 0:
    nav.move_one_step()
    print(nav.grid)
