#Cary Hooper
#Advent of Code
##Day 5 Part 1

#dec-----bin
def dec2bin(value):
    return bin(int(value.decode())).encode()[2:]
def bin2dec(value):
    return str(int(value.decode(),base=2)).encode()

def get_unique_seatid(row,seat):
	return (int(row) * 8) + int(seat)

def find_seat_number(seat_string):
	seat_string = seat_string.replace("L","0").replace("R","1")
	binary_string = "00000" + seat_string
	return bin2dec(binary_string.encode())

def find_row_number(row_string):
	row_string = row_string.replace("F","0").replace("B","1")
	binary_string = "0" + row_string
	return bin2dec(binary_string.encode())

file = open("boarding_passes.txt","r")
boarding_passes = file.readlines()

highest_pass = 0
other_seats = list()

for boarding_pass in boarding_passes:
	boarding_pass = boarding_pass.strip()
	print(f"Analyzing: {boarding_pass}")
	row = find_row_number(boarding_pass[0:7]).decode()
	print(f"row {row}")
	seat = find_seat_number(boarding_pass[-3:]).decode()
	print(f"seat {seat}")
	uid = get_unique_seatid(row,seat)
	print(uid)
	print(type(uid))

	print(f"Boarding Pass: {boarding_pass} | Row {row}, Seat {seat}, Unique Seat ID {uid}")
	other_seats.append(uid)
	if uid > highest_pass:
		highest_pass = uid

print(f"The highest unique seat ID is {highest_pass}")

#Part 2
sorted_seats = sorted(other_seats)
last_seat = 0
for seat in sorted_seats:
	if seat != last_seat +1:
		print(f"Seems to have skipped between {last_seat} and {seat}")
	last_seat = seat