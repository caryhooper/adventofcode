#Cary Hooper
#Advent of Code
##Day 1 Part 1
from itertools import combinations
from math import prod

def do_product(args):
	product = prod(args)
	print(f"Multiplying {args}")
	return product

def find_sum(number_list):
	for number in number_list:
		#print(f"Looking for {number}")
		addend = 2020 - number
		#print(f"\tAddend is {addend}")
		if addend in number_list:
			answer_list = list()
			answer_list.append(number)
			answer_list.append(addend)
			return do_product(answer_list)

number_file = open("numbers.txt","r").readlines()
number_list = list()
for line in number_file:
	line = line.strip()
	number = int(line)
	number_list.append(number)

answer = find_sum(number_list)
print(f"The product of the two addends of 2020 is: {answer}")

##Day 1 Part 2
def do_sum(args):
	#print(f"Adding: {args}")
	return sum(list(args))

def find_sum_n(number_of_elements,iterable):
	combinations_list = list(combinations(iterable,number_of_elements))
	for comb in combinations_list:
		#print(comb)
		if do_sum(list(comb)) == 2020:
			return do_product(list(comb))

answer = find_sum_n(3,number_list)
print(f"The product of the three addends of 2020 is: {answer}")