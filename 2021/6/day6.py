#!/usr/bin/env python3
#Advent of Code Day 4
import sys
sys.path.append("..")

import submarine
sub = submarine.Submarine()

import creatures.lanternfish
#Fish counter...
#Load initial state of fish
file = open('input.txt','r')
all_fish = list()

seed = [all_fish.append(creatures.lanternfish.Lanternfish(age=int(age))) for age in file.read().strip().split(',')]
file.close()

#Part 1
#Observe for X days
for day in range(1,81):
	print(f"Beginning of Day {day}. There are {len(all_fish)} fish.")
	#First add all new fish
	new_fish = [creatures.lanternfish.Lanternfish() for fish in all_fish if fish.cycle_counter == 0]
	#New fish initialized with 9 days.
	[all_fish.append(f) for f in new_fish]
	#Age all fish 1 day
	[fish.age() for fish in all_fish]
	print(f"End of Day {day}.  There are {len(all_fish)} fish.")


#Part 2
#Need better data structure to scale.
#Initialize fish dict keeping track of cycle_age and count
fish = dict()
for i in range(0,9):
	fish[i] = 0
for age in open('input.txt','r').read().strip().split(','):
	fish[int(age)] += 1

for day in range(1,257):
	#Add new fish last since we're adding with 8 days
	new_fish = fish[0]
	for age in range(0,len(fish.keys()) - 1):
		fish[age] = fish[age+1]
	#New fish born
	fish[8] = new_fish
	#Cycle Repeats
	fish[6] += new_fish

total_fish = sum(fish.values())
print(f"There will be {total_fish} after Day 256.")


