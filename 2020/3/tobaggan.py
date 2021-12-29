#Cary Hooper
#Advent of Code
##Day 3 Part 1
import sys

#Added function for Part 2
def check_trees(slopex, slopey):
	#Handle Slope
	elevation_lines = open("trees.txt","r").readlines()

	y_coord = 0
	x_coord = 0
	tree_counter = 0
	for elevation in elevation_lines:
		if y_coord % slopey == 0:
			elevation = elevation.strip() * 77
			print(f"x_coord is {x_coord} and tree_counter is {tree_counter}")
			if elevation[x_coord] == "#":
				#print(f"{elevation[x_coord]}")
				tree_counter += 1
			x_coord += slopex
		y_coord += 1
	print(f"There are {tree_counter} trees for Right {slopex}, down {slopey}.")
	return tree_counter

from math import prod

def do_product(args):
	product = prod(args)
	print(f"Multiplying {args}")
	return product

tree_list = list()
tree_list.append(check_trees(1,1))
tree_list.append(check_trees(3,1))
tree_list.append(check_trees(5,1))
tree_list.append(check_trees(7,1))
tree_list.append(check_trees(1,2))

product = do_product(tree_list)
print(f"The product of all the trees encountered for each slope is {product}")