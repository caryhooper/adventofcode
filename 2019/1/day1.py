import math

#part2 

def total_fuel_required(mass):
    fuel_needed = 0
    if mass > 6:
        fuel_needed = math.floor(mass/3) - 2
        fuel_needed += total_fuel_required(fuel_needed)
    return fuel_needed

file = open('1/input.txt','r')
total = 0
for line in file.readlines():
    line = line.strip()
    mass = int(line,10);
    total += total_fuel_required(mass)
print(f"{total} fuel needed for liftoff.")

