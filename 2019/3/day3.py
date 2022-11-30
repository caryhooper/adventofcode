#Draw paths that cross at various points and find the Manhattan 
#    distance from origin

#input2.txt
# R75,D30,R83,U83,L12,D49,R71,U7,L72
# U62,R66,U55,R34,D71,R55,D58,R83


class Point:
    def __init__(self, x: int, y: int):
        self.x = x
        self.y = y

    def get_manhattan_distance(self):
        return abs(self.x) + abs(self.y)

class Path:
    def __init__(self):
        self.origin = Point(0,0)
        self.path = list()
        # self.path.append(self.origin)
        self.current_location = self.origin

    def travel_vector(self, direction: str, distance: int):
        if direction == "R":
            #y is constant for R/L
            for x in range(1,distance + 1):
                self.path.append(Point(self.current_location.x + x,self.current_location.y))
        elif direction == "L":
            for x in range(1,distance + 1):
                self.path.append(Point(self.current_location.x - x,self.current_location.y))
        elif direction == "U":
            #x is constant for U/D
            for y in range(1,distance + 1):
                self.path.append(Point(self.current_location.x, self.current_location.y + y))
        elif direction == "D":
            for y in range(1,distance + 1):
                self.path.append(Point(self.current_location.x, self.current_location.y - y))
        else:
            print(f"Illegal direction provided: {direction}")
        #Set current location to last point of the path.
        self.current_location = self.path[-1]

    def parse_from_line(self, input_info: str):
        for vector in input_info.split(","):
            vector = vector.strip()
            direction = vector[0]
            magnitude = int(vector[1:],10)
            self.travel_vector(direction,magnitude)

def get_manhattan_distance(our_point: tuple):
    x,y = our_point
    return abs(x) + abs(y)

def calculate_steps(our_point: tuple, first: Path, second: Path):
    first_step_count = 0
    second_step_count = 0
    x,y = our_point
    for step in first.path:
        first_step_count+= 1
        if(step.x == x and step.y == y):
            break
    for step in second.path:
        second_step_count+= 1
        if(step.x == x and step.y == y):
            break
    return first_step_count + second_step_count


def find_closest_intersection(first: Path, second: Path):
    first_tuples = set()
    second_tuples = set()
    [first_tuples.add((i.x,i.y)) for i in first.path]
    [second_tuples.add((i.x,i.y)) for i in second.path]

    common_points = first_tuples.intersection(second_tuples)
    print(f"Common points\n{common_points}")
    closest_distance = min(list(map(lambda point: get_manhattan_distance(point), common_points)))
    print(f"The closest intersection to the origin is {closest_distance} away.")

    fewest_steps  = min(list(map(lambda point: calculate_steps(point, first, second), common_points)))
    print(f"The intersection requiring the fewest total steps is {fewest_steps} away.")

    return fewest_steps



with open('input.txt','r') as f:
    content = f.read()

pathA = Path()
pathB = Path()
input1,input2 = content.split("\n")
pathA.parse_from_line(input1)
pathB.parse_from_line(input2)

# for point in pathA.path:
#     print(f"({point.x},{point.y})")

# for point in pathB.path:
#     print(f"({point.x},{point.y})")


answer_part1 = find_closest_intersection(pathA,pathB)







