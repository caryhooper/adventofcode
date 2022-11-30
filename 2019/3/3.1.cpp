#include <cstdlib>
#include <cstdio>
#include <fcntl.h>
#include <string>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream>

#define BUFSIZE 2^20

class Point {
    public:
        int64_t x;
        int64_t y;

        Point(){
            this->x = 0;
            this->y = 0;
        }

        Point(int x, int y){
            this->x = x;
            this->y = y;
        }

        ~Point(){}

        int64_t calculateManhattanDistance(){
            return std::abs(this->x) +  std::abs(this->y);
        }
};

typedef struct {
    char direction;
    int magnitude;
} vector;

class Path {
    public: 
        Point origin;
        Point current_position;
        Point * path;
        uint64_t number_of_steps;
        std::vector<vector> instructions;
        int64_t number_of_instructions;

        Path(){
            this->origin = Point(0,0);
            this->current_position = this->origin;
            this->number_of_instructions = 0;
            this->number_of_steps = 0;
            this->path = new Point[3];
        }
        ~Path(){}

        void setCurrentPosition(){
            this->current_position.x = this->path[this->number_of_steps - 1 ].x;
            this->current_position.y = this->path[this->number_of_steps - 1 ].y;
        }

        void addInstruction(char dir, int mag){
            this->instructions[this->number_of_instructions].direction = dir;
            this->instructions[this->number_of_instructions].magnitude = mag;
            this->number_of_instructions++;
        }

        void drawPath(){
            //Not implemented.
            for(int i = 0; i < number_of_instructions; i++){
                vector v = this->instructions[i];
                switch(v.direction) {
                    //Movement in X for R/L
                case 'R'  :
                    for(int j = 1; j <= v.magnitude; j++){
                        this->path[this->number_of_steps] = Point(this->current_position.x + j, this->current_position.y);
                        this->number_of_steps++;
                    }
                    break; /* optional */
                case 'L'  :
                    for(int j = 1; j <= v.magnitude; j++){
                        this->path[this->number_of_steps] = Point(this->current_position.x - j, this->current_position.y);
                        this->number_of_steps++;
                    }                    break; /* optional */
                    //Movement in Y for U/D
                case 'U':
                    for(int j = 1; j <= v.magnitude; j++){
                        this->path[this->number_of_steps] = Point(this->current_position.x, this->current_position.y + j);
                        this->number_of_steps++;
                    }
                    break;
                case 'D':
                    for(int j = 1; j <= v.magnitude; j++){
                        this->path[this->number_of_steps] = Point(this->current_position.x, this->current_position.y - j);
                        this->number_of_steps++;
                    }
                    break;
                default : /* Optional */
                    printf("Error.  Wrong char provided for vector direction: %c\n",v.direction);
                } 
                this->setCurrentPosition();
            }
            this->number_of_steps--;
        }

        int64_t stepsToPoint(Point pt){
            //walk the path
            for(int64_t i = 0; i < (int64_t) this->number_of_steps; i++){
                if(this->path[i].x == pt.x && this->path[i].y == pt.y){
                    return i;
                }
            }
            return 0;
        }

        // Point findIntersectingPoint(Path * another_path){
        //     for(int our_step = 0; our_step < this->number_of_steps; our_step++){
        //         for(int another_step = 0; another_step < another_path->number_of_steps; another_step++){
        //             if(
        //                 this->path[our_step].x == another_path->path[another_step].x &&
        //                 this->path[our_step].y == another_path->path[another_step].y
        //             ){
        //                 return this->path[our_step];
        //             }
        //         }
        //     }
        //     return Point(0,0);
        // }
};

int main(){
    std::ifstream file_stream("input2.txt",std::ios::in);

    //Initialize
    Path * path_array[2];
    for(int i = 0; i < 2; i++){
        Path * this_path = new Path();
        path_array[i] = this_path;
        // Path our_path = Path();
    }

    //Init file reader variables
    int path_counter = 0;
    int instruction_counter = 0;
    // char * line = (char *) calloc(1, sizeof(char)*BUFSIZE);
    std::string line;
    char dir;
    std::string mag_string;
    int mag = 0;
    bool new_instruction = true;

    while( file_stream ){
        std::getline(file_stream,line,'\n');
        std::cout << line;
        for(size_t i = 0; i < line.length(); i++){
            if(line[i] == ',' || line[i] == '\n'){
                //Here, we have the vector
                mag = std::stoi(mag_string);
                path_array[path_counter]->addInstruction(dir,mag);
                instruction_counter++;
                new_instruction = true;
                mag_string.erase();
                continue;
            }
            if(new_instruction == true){
                new_instruction = false;
                dir = line[i]; //letter (aka direction)
                continue;
            }
            strncat(&mag_string[0],&line[i],1);
        }

        std::cout  << "Initalized path object at pointer " << path_array[path_counter];
        instruction_counter = 0;
        path_counter++;
        line.erase();
    }

    //Here all Path() objects (should be two) have been created.  Now we need to do the calculations.
    //(1) Where do they intersect?
    path_array[0]->drawPath(); //Not implemented
    path_array[1]->drawPath(); //Not implemented
    int64_t least_manhattan_distance = INT64_MAX;
    int64_t manhattan_distance = 0;
    int64_t step_distance = 0;
    int64_t least_step_distance = INT64_MAX;

    for(int i = 0; i < (int64_t) path_array[0]->number_of_steps; i++){
        for(int j = 0; j < (int64_t) path_array[1]->number_of_steps; j++){
            if(
                path_array[0]->path[i].x == path_array[1]->path[j].x && 
                path_array[0]->path[i].y == path_array[1]->path[j].y
            ){
                //Find the manhattan distance (part 1)
                manhattan_distance = path_array[0]->path[i].calculateManhattanDistance();
                if(manhattan_distance < least_manhattan_distance){
                    least_manhattan_distance = manhattan_distance;
                }

                //Find the least steps
                step_distance = path_array[0]->stepsToPoint(path_array[0]->path[i]) + path_array[1]->stepsToPoint(path_array[1]->path[j]);
                if(step_distance < least_step_distance){
                    least_step_distance = step_distance;
                }

            }
        }
    }

    //Part 1
    printf("The closest intersection to the origin has a manhattan distance of %ld\n",least_manhattan_distance);
    printf("The closest intersection to the origin has a total step distance of %ld\n",least_step_distance);
}


