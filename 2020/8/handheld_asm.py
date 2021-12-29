#Cary Hooper
#Advent of Code
##Day 8 Part 1
##Day 8 Part 2 gives wrong answer
import sys

def execute_instructions(new_instructions):
	#init loop detector
	loop_detector = [0] * num_instructions
	#init acc register and instruction pointer
	acc = 0
	eip = 0

	#Execute instructions
	#0) Check if next instruction has been executed already
	#1) Parse instruction
	#2) Do command
	#2a) Modify registers
	#print(loop_detector)
	while eip < num_instructions and loop_detector[eip] == 0:#0
		#1
		# next_instr_parts = instruction_strings[eip].split()
		next_instr_parts = new_instructions[eip].split()
		instr = next_instr_parts[0]
		modifier = int(next_instr_parts[1])
		#print(f"{instr} {modifier}.  EIP {eip} ACC {acc}")

		#Update loop_detector before executing instruction
		loop_detector[eip] = 1
		#2
		if instr == "jmp":
			eip += modifier
		elif instr == "acc":
			acc += modifier
			eip += 1
		elif instr =="nop":
			eip += 1
		else:
			print(f"Error.  Bad Instr. INSTR: {instr} {modifier}. EIP: {eip}, ACC: {acc}.")

	#print(f"The value of accumulator is {acc} before an instruction has been executed a second time.")
	if eip == num_instructions:
		print(f"Success!  Program booted successfully. The value of accumulator is {acc}.  EIP {eip}")
	elif eip > num_instructions:
		print(f"Failed!  eip is now {eip}")
	else:
		print(f"Infinite loop at instr {eip}: {loop_detector[eip]}.  ACC {acc}")


#Basically, we're going to create a basic mini ASM computer in Python.  LOL
file = open("code.txt","r")
instruction_strings = file.readlines()
num_instructions = len(instruction_strings)
#init instruction array
instructions = [0] * num_instructions

count = 0
# print(instruction_strings)
for instr in instruction_strings:
	instr = instr.strip()
	instructions[count] = instr 
	count += 1
	# print(instr)


#Day 8 Part 1
execute_instructions(instructions)

for i in range(0,num_instructions):
	#Cycle through all instructions and mutate
	new_instructions = instructions
	if "acc" in new_instructions[i]:
		continue
	elif "nop" in new_instructions[i]:
		new_instructions[i] = new_instructions[i].replace("nop","jmp")
		print(f"Changing instruction {i} to {new_instructions[i]}")
	elif "jmp" in new_instructions[i]:
		new_instructions[i] = new_instructions[i].replace("jmp","nop")
		print(f"Changing instruction {i} to {new_instructions[i]}")
	else:
		print("Error: nothing to change.")

	execute_instructions(new_instructions)
