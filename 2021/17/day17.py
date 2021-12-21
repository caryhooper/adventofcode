#!/usr/bin/env python3
#Advent of Code Day 17 "Trick Shot"
import sys
sys.path.append("..")

import submarine
sub = submarine.Submarine()

file = open('input.txt','r')
cannon = sub.weapons["probe_cannon"]
cannon.parse_target_area(file.readlines())

velocity = (2,1)
success,y_max,path,failure_message = cannon.fire(velocity)
print(f"Fired at velocity {velocity} with path {path}")

# sys.exit(0)
greatest_y = y_max
greatest_y_velocity = velocity
while velocity[1] < 200:
    x_v,y_v = velocity
    success,y_max,path,failure_message = cannon.fire(velocity)
    # print(f"Fired at velocity {velocity} with path {path}")
    if failure_message == ',':
        y_v += 1
    x_msg,y_msg = failure_message.split(',')
    if x_msg == 'inc':
        x_v += 1
    elif x_msg == 'dec':
        x_v -= 1
    else:
        pass
    if y_msg == 'inc':
        y_v += 1
    if success:
        if y_max > greatest_y:
            greatest_y = y_max
            greatest_y_velocity = velocity
    velocity = x_v,y_v

# Max y was 5778 with velocity (18, 107)
print(f"Max y was {greatest_y} with velocity {greatest_y_velocity}")

