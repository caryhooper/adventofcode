#Cary Hooper
#Advent of Code
##Day 6 Part 1
import string


file = open("customs.txt","r")
forms = file.read().split("\n\n")

plane_total = 0
plane_total_pt2 = 0
for form_group in forms:
	group_answers = set()
	form_group = form_group.strip()
	papers = form_group.split("\n")
	#Pt 2 addition
	same_answers = set(string.ascii_lowercase)
	for paper in papers:
		this_paper_set = set()
		paper = paper.strip()
		for answer in paper:
			answer = answer.strip()
			#This is for pt2
			this_paper_set.add(answer)
			group_answers.add(answer)
		#This if for Pt2
		print(f"Same Answers: {same_answers} | This_Paper_set {this_paper_set}")
		same_answers = same_answers - (same_answers - this_paper_set)
		print(same_answers)
	group_total = len(group_answers)
	print(f"---FORM GROUP---\n{form_group}")
	#print(f"Results: {group_answers}")
	plane_total += group_total
	#print(f"Group Total: {group_total}")
	plane_total_pt2 += len(same_answers)

print(f"The sum of the group customs counts is {plane_total}")

print(f"The sum of the group customs counts (new rules) is {plane_total_pt2}")

