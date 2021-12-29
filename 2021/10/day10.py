#!/usr/bin/env python3
#Advent of Code Day 10 "Syntax Scoring"
import sys
sys.path.append("..")


import submarine
sub = submarine.Submarine()

#Part1
file = open("input.txt","r")
completion_strings = sub.navigator.correct_syntax(file.readlines())
error_chars = sub.navigator.first_error_chars

score = 0
score_key = {
    ")": 3,
    "]": 57,
    "}": 1197,
    ">": 25137}

for e in error_chars:
    score += score_key[e]
print(f"The error score is {score}")

#Part 2
score_key = {
    ")": 1,
    "]": 2,
    "}": 3,
    ">": 4}

scores = list()

for progress in completion_strings:
    print(progress)
    score = 0
    for item in progress:
        score *= 5
        score += score_key[item] 
    scores.append(score)
import statistics
median = statistics.median(scores)

print(f"The completion score is {median}")