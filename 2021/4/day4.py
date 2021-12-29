#!/usr/bin/env python3
#Advent of Code Day 4
import sys
sys.path.append("..")

import submarine
sub = submarine.Submarine()

file = open('input.txt','r')

bingo = sub.entertainment["bingo"]
bingo.initialize(file.readlines())

print(f"There are {len(bingo.boards)} boards initialized.")
bingo.play()
first_win_round = 999
last_win_round = 0
for board in bingo.boards:
	if board.win_round < first_win_round:
		first_win_round = board.win_round
	if board.win_round > last_win_round:
		last_win_round = board.win_round
for board in bingo.boards:
	if board.win_round == first_win_round:
		print(f"The final score of the first winner will be {board.final_score} in round {board.win_round}")
	if board.win_round == last_win_round:
		print(f"The final score of the last winner will be {board.final_score} in round {board.win_round}")

