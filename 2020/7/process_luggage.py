#Cary Hooper
#Advent of Code
##Day 7 Part 1
##Note - this code is unfinished.   Doesn't result in the correct answer.

def parse_rules(filename):
	#Parses rules file, returns dict with all the rules for each type of bag.
	file = open(filename,"r")
	lines = file.readlines()
	rules = dict() #Dict to hold all our luggage rules
	for line in lines:
		bags_within = dict() #Dict to hold all bags required inside and the number of bags.
		line = line.strip()
		parts = line.split("contain")
		subject_bag = parts[0][:-6].strip() #Subject of the rule
		if "no other bags" not in parts[1]:
			contained_bags_list = parts[1].split(",")
			for contained_bag in contained_bags_list:
				#Cycle through all contained bags and save in bags_within dict.
				contained_bag = contained_bag[:-4].strip()
				if contained_bag[-1] == "b":
					contained_bag = contained_bag[:-1].strip()
				parts = contained_bag.split(" ",1)
				contained_bag_name = parts[1].strip()
				contained_bag_number = parts[0].strip()
				bags_within[contained_bag_name] = contained_bag_number
		rules[subject_bag] = bags_within
		#print(f"{subject_bag}:{bags_within}")
	return rules

rules_dict = parse_rules("luggage.txt")
#print(rules_dict)


def find_container_bags(containers_set):
	#Given a set of bag types, return a set of bags that could possibly contain them.	
	new_containers_set = set()
	for thing in containers_set:
		new_containers_set.add(thing)
	for bag_type in containers_set:
		print(f"Trying {bag_type}")
		for bag,contained_dict in rules_dict.items():
			if bag_type in contained_dict.keys():
				print(f"{bag} bags must directly contain a {bag_type}.")
				new_containers_set.add(bag)
	if len(containers_set) == len(new_containers_set):
		print("Exiting recursive routine")
		return containers_set
	else:
		return find_container_bags(new_containers_set)



starting_bag = set()
starting_bag.add("shiny gold")
containers_set = find_container_bags(starting_bag)
print(f"{len(containers_set)} different bags may contain a shiny gold")
# print(containers_set)
print("\n\n")
print(sorted(containers_set))