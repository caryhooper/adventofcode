#Cary Hooper
#Advent of Code
##Day 2 Part 1

file = open("passwords.txt","r")
lines = file.readlines() 
valid_passwords_oldpolicy = 0
valid_passwords_newpolicy = 0

for line in lines:
	parts = line.split(':')
	policy = parts[0].strip()
	password = parts[1].strip()
	parts = policy.split(" ")
	num_range = parts[0].strip()
	match_letter = parts[1].strip()
	parts = num_range.split("-")
	lower = int(parts[0])
	upper = int(parts[1])

	occurrences = password.count(match_letter)
	if occurrences >= lower and occurrences <= upper:
		valid_passwords_oldpolicy += 1

	#NewPolicy Applied - Part2	
	#Logical XOR
	if (password[lower - 1] == match_letter and not password[upper - 1] == match_letter) or (not password[lower - 1] == match_letter and  password[upper - 1] == match_letter):
		valid_passwords_newpolicy += 1

print(f"{valid_passwords_oldpolicy} total passwords are valid with the old policy.")

print(f"{valid_passwords_newpolicy} total passwords are valid with the new policy.")
