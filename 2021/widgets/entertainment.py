#!/usr/bin/env python3
import sys
class Bingo:
	def __init__(self):
		# self.final_score = 0
		self.random_numbers = list()
		self.boards = list()
		self.round = 0
		pass

	def initialize(self, input_lines):
		#Expect comma-separated "random" numbers in the first line
		#Expect a newline to separate follow-on boards (lines)
		board_input = []
		for line in input_lines:
			if "," in line.strip():
				numbers = line.split(',')
				[self.random_numbers.append(num.strip()) for num in numbers]
			else:
				#Needs error handling
				#Save the next four lines in an array of arrays for the gameboard
				#And initialize a board for each.
				if line == "\n" and len(board_input) == 5:
					# print("Initializing Bingo Board...")
					self.boards.append(Bingo_Board(board_input))
					board_input = []
				else:
					line = ' '.join(line.strip().split())
					line_items = [num for num in  line.split(' ')]
					if len(line_items) != 5:
						print(f"Line {line} was not processed: {line_items}")
					else:
						board_input.append(line_items)

	def play(self):
		for number in self.random_numbers:
			#Call a number.
			print(f"Calling number {number}...")
			for board in self.boards:
				if not board.winner:
					board.update_board(number, self.round)
					if board.winner == True:
						print(f"Found a winning board with a final_score of {board.final_score}")
						# return True
			self.round += 1

class Bingo_Board:
	def __init__(self,input_lines):
		#initialize board
		self.board_state = [['x'] * len(input_lines) for x in range(len(input_lines))]
		for i in range(0,len(input_lines)):
			for j in range(0,len(input_lines[i])):
				self.board_state[i][j] = input_lines[i][j]
		self.winner = False
		self.final_score = 0
		self.win_round = 999

	def update_board(self,number_called, round_num):
		#Iterate through each grid square and if the dict key matches number_called, update to True
		self.last_number_called = number_called
		self.this_round = round_num
		for i in range(0,len(self.board_state)):
			for j in range(0,len(self.board_state[i])):
				if number_called == self.board_state[i][j]:
					self.board_state[i][j] = 'x'
		self.check_winner()

	def check_winner(self):
		#Check horizontally
		#Garbage code but it works.
		row_winner = False
		for i in range(0,len(self.board_state)):
			if self.board_state[i][0] == self.board_state[i][1] and \
				self.board_state[i][0] == self.board_state[i][2] and \
				self.board_state[i][0] == self.board_state[i][3] and \
				self.board_state[i][0] == self.board_state[i][4]:
				row_winner = True

		col_winner = False
		for i in range(0,len(self.board_state)):
			if self.board_state[0][i] == self.board_state[1][i] and \
				self.board_state[0][i] == self.board_state[2][i] and \
				self.board_state[0][i] == self.board_state[3][i] and \
				self.board_state[0][i] == self.board_state[4][i]:
				col_winner = True

		if row_winner or col_winner:
			self.winner = True
			self.win_round = self.this_round
			print(f"BINGO! Last called number: {self.last_number_called}")
			print(self.board_state)
			self.calc_finalscore()

	def calc_finalscore(self):
		this_sum = 0
		for i in range(0,len(self.board_state)):
			for j in range(0,len(self.board_state[i])):
				if self.board_state[i][j] != 'x':
					this_sum += int(self.board_state[i][j])
		self.final_score = this_sum * int(self.last_number_called)