

instructions = list()

file = open('2/input.txt','r')
index = 0

content = file.read()
for num in content.split(","):
    num = num.strip()
    instructions.append(int(num,10))

instructions[1] = 12
instructions[2] = 2

index = 0
while instructions[index] != 99:
    instruction = instructions[0]
    if instructions[index] == 1:
        instructions[instructions[index+3] ]= instructions[instructions[index + 1]] + instructions[instructions[index + 2]]
    elif instructions[index] == 2:
        instructions[instructions[index+3] ]= instructions[instructions[index + 1]] * instructions[instructions[index + 2]]
    else:
        print(f"Illegal instruction: {instructions[index]} at index {index}")
    index += 4

print(f"{instructions[0]} is the first value before the crash.")
