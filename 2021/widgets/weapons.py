class ProbeCannon:
    def __init__(self):
        self.location = (0,0)
        self.target_area = list()
        pass

    def parse_target_area(self, lines):
        #target area: x=20..30, y=-10..-5
        for line in lines:
            line = line.strip().replace(" ","").replace('..','|')
            if "targetarea" in line:
                ranges = line.split(':')[1]
                x_range,y_range = ranges.split(',')
                x1,x2 = x_range.split('|')
                x1 = x1.split('=')[1]
                self.x_max = int(x2)
                self.x_min = int(x1)
                y1,y2 = y_range.split('|')
                y1 = y1.split("=")[1]
                self.y_min = int(y1)
                self.y_max = int(y2)

        for i in range(self.x_min,self.x_max + 1):
            for j in range(self.y_min, self.y_max + 1):
                coord = (i,j)
                self.target_area.append(coord)

    def fire(self,velocity):
        path = list()
        path.append(self.location)
        curr_x,curr_y = self.location
        in_target_area = False
        while curr_x < self.x_max and curr_y > self.y_min and in_target_area == False:
            #Track progress while probe has a chance to land within target area.
            # The probe's x position increases by its x velocity.
            curr_x += velocity[0]
            # The probe's y position increases by its y velocity.
            curr_y += velocity[1]
            # Due to drag, the probe's x velocity changes by 1 toward the value 0; that is, it decreases by 1 if it is greater than 0, increases by 1 if it is less than 0, or does not change if it is already 0.
            if velocity[0] > 0:
                velocity = (velocity[0] - 1, velocity[1])
            elif velocity[0] < 0:
                velocity = (velocity[0] + 1, velocity[1])
            else:
                pass
            # Due to gravity, the probe's y velocity decreases by 1.
            velocity = (velocity[0], velocity[1] - 1)

            if (curr_x,curr_y) in self.target_area:
                in_target_area = True
            path.append((curr_x,curr_y))
        #Calculate largest y in path
        max_alt = max([y for _,y in path])

        #Calculate what failed...
        failure_message = ","
        if not in_target_area:
            if curr_x > self.x_max:
                failure_message = "sub," #decrease x
            elif curr_x < self.x_min:
                failure_message = "inc," #increase x
            else:
                failure_message = "," #Do nothing
            if curr_y > self.y_max:
                pass
            elif curr_y < self.y_min:
                failure_message += "inc"
            else:
                failure_message += ""
          
            
# ___|___|___
# ___|___|___
#    |   |
#
#
#
#
#
#


        return in_target_area,max_alt,path,failure_message
