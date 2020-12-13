#Cary Hooper
#Advent of Code
##Day 4 Part 1
import sys, re
#Valid Fields
    # byr (Birth Year)
    # iyr (Issue Year)
    # eyr (Expiration Year)
    # hgt (Height)
    # hcl (Hair Color)
    # ecl (Eye Color)
    # pid (Passport ID)
    # cid (Country ID) / optional for North Pole Credentials
DEBUG = False

def log(message):
	if DEBUG:
		print(message)

def parse_passports(filename):
	#Parse input into a list of dicts
	passports = list()
	file = open(filename,"r")
	content = file.read()
	entries = content.split("\n\n")
	#Split blob into passports
	for entry in entries:
		#Split passports into field codes
		entry = entry.strip()
		entry = entry.replace('\n',' ')
		fields = entry.split(' ')
		passport = dict()
		for field in fields:
			field = field.strip()
			parts = field.split(':')
			passport[parts[0]] = parts[1]
		passports.append(passport)
	return passports

def passport_has_valid_fields(passport):
	valid_passport_template = ['byr','iyr','eyr','hgt','hcl','ecl','pid']
	valid_passport = all(elem in passport.keys() for elem in valid_passport_template)
	if valid_passport:
		log(f"Valid Fields: {passport}")
	return valid_passport

def validate_passport_info(passport):
	# byr (Birth Year) - four digits; at least 1920 and at most 2002.
    birthyear = int(passport["byr"])
    if birthyear < 1920 or birthyear > 2002:
    	log(f"Failed birthyear validation. {birthyear}")
    	return False
    # iyr (Issue Year) - four digits; at least 2010 and at most 2020.
    issueyear = int(passport["iyr"])
    if issueyear < 2010 or issueyear > 2020:
    	log(f"Failed issueyear validation. {issueyear}")
    	return False
    # eyr (Expiration Year) - four digits; at least 2020 and at most 2030.
    expyear = int(passport["eyr"])
    if expyear < 2020 or expyear > 2030:
    	log(f"Failed expiration year validation. {expyear}")
    	return False
    # hgt (Height) - a number followed by either cm or in:
    #     If cm, the number must be at least 150 and at most 193.
    #     If in, the number must be at least 59 and at most 76.
    height_string = passport["hgt"]
    number = int(height_string[:-2])
    if "cm" not in height_string and "in" not in height_string:
    	return False
    if "cm" in height_string:
    	if number < 150 or number > 193:
    		log(f"Failed height validation. {height_string}")
    		return False
    if "in" in height_string:
    	if number < 59 or number > 76:
    		log(f"Failed height validation. {height_string}")
    		return False
    # hcl (Hair Color) - a # followed by exactly six characters 0-9 or a-f.
    haircolor = passport["hcl"]
    color_match = re.match("^#[0-9a-f]{6}$",haircolor)
    if color_match == None:
    	log(f"Failed haircolor validation. {color_match} {haircolor}")
    	return False
    # ecl (Eye Color) - exactly one of: amb blu brn gry grn hzl oth.
    eyecolor = passport["ecl"]
    if eyecolor not in ["amb","blu","brn","gry","grn","hzl","oth"]:
    	log(f"Failed eyecolor validation. {eyecolor}")
    	return False
    # pid (Passport ID) - a nine-digit number, including leading zeroes.
    passportID = passport["pid"]
    passport_id_match = re.match("^[0-9]{9}$",passportID)
    if passport_id_match == None:
    	log(f"Failed passport ID validation. {passport_id_match} {passportID}")
    	return False
    # cid (Country ID) - ignored, missing or not.
    log("Passport is valid.")
    return True



passports = parse_passports("passports.txt")
valid_passports = 0
for passport in passports:
	if passport_has_valid_fields(passport):
		if validate_passport_info(passport):
			print(f"Valid Passport: {passport}")
			valid_passports += 1

print(f"In total, {valid_passports} passports are valid")
