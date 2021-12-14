#!/usr/bin/env python3
#Advent of Code Day 14 "Extended Polymerization"
import sys
sys.path.append("..")

import submarine
sub = submarine.Submarine()

filename = "input.txt"
#Part 1
man = sub.manual

file = open(filename,"r")
man.parse_rules(file.readlines())

print(man.polymer_template)
man.do_steps(number_of_steps=10)
most_common,least_common = man.find_most_and_least_common_elements()
difference = most_common - least_common
print(f"The resultant difference after {man.step} steps is {difference}")
man.step = 0
file.close()

#Part 2 Electric Bugalooo
file = open(filename,"r")
man.parse_rules(file.readlines())

man.do_steps(number_of_steps=40,do_better = True)
letter_count_list = list(man.letters_count.values())
letter_count_list.sort()
# print(f"Letter Count List: {letter_count_list}")
most_common = letter_count_list[-1]
least_common = letter_count_list[0]
difference = most_common - least_common
print(f"The resultant difference after {man.step} steps is {difference}")