#include <string> 
#include <iostream> //
#include <algorithm> //reverse
#include <fstream> //ifstream
#include <map> //map
#include <vector>
#include <set>
#include <cmath>
#include <cassert>
#include <map>
#include <limits>
#include <numeric>
#include "../helpers.hpp"

using namespace advent_of_code;

// enum pipe_t {
//     NS,
//     EW,
//     NE,
//     NW,
//     SW,
//     SE,
//     NA
// };

class Pipe{
    public:
        Pipe(char c, size_t x, size_t y){
            pipe_char_ = c;
            x_ = x;
            y_ = y;
            //type_ = get_pipe_type(c);
        }
        char pipe_char_; 
        std::vector<Pipe *> neighbors_;
        size_t x_ = 0;
        size_t y_ = 0; 
        //pipe_t type_;
        void get_pipe_type(char c){
            switch(c){
                case '|':
                    return;
                case '-':
                    return;  
                case 'L':
                    return;
                case 'J':
                    return;  
                case '7':
                    return;
                case 'F':
                    return;  
                case '.':
                    return;
                case 'S':
                    return;            
            }

        }

};

class PipeGrid{
    public:
        PipeGrid(){

        }

        void calc_x(){
            //x is the largest sub-vector
            x_ = 0; 
            size_t size_row;
            for(std::vector<Pipe *> row: grid){
                size_row = row.size();
                if(size_row  > x_){
                    x_ = size_row;
                }
            }
        }
        void calc_y(){
            y_ = grid.size();
        }

        void parse_from_file(std::string filename){
            std::ifstream input_file(filename);
            std::string line;
            size_t x;
            size_t y = 0;
            while(std::getline(input_file,line)){
                x = 0;
                std::vector<Pipe * > row;
                for(char c : line){
                    row.push_back(new Pipe(c,x,y));
                    x += 1;
                }
                grid.push_back(row);
                y += 1;
            }
            calc_x();
            calc_y();
            infer_pipe_type();
        }

        void infer_pipe_type(){
            Pipe * this_pipe = NULL;
            size_t x,y;
            for(y = 0; y < y_; y++){
                for(x = 0; x < x_; x++){
                    this_pipe = grid[y][x]; 
                    if(this_pipe->pipe_char_ == 'S'){
                        x = x_;
                        y = y_;
                        start_ = this_pipe;
                    }
                }
            }
            assert(this_pipe != NULL);
            //check up
            x = this_pipe->x_;
            y = this_pipe->y_;
            bool up = false;
            bool down = false;
            bool left = false;
            bool right=false;
            if(y > 0){
                if(grid[y-1][x]->pipe_char_ == 'F' || grid[y-1][x]->pipe_char_ == '7'  || grid[y-1][x]->pipe_char_ == '|' ){
                    up = true;
                }
            }
            //check down
            if(y < y_){
                if(grid[y+1][x]->pipe_char_ == 'L' || grid[y+1][x]->pipe_char_ == 'J'  || grid[y+1][x]->pipe_char_ == '|' ){
                    down = true;
                }
            }
            //check left
            if(x > 0){
                if(grid[y][x-1]->pipe_char_ == 'F' || grid[y][x-1]->pipe_char_ == 'L'  || grid[y][x-1]->pipe_char_ == '-' ){
                    left = true;
                }
            }
            //check right
            if(x < x_){
                if(grid[y][x+1]->pipe_char_ == 'J' || grid[y][x+1]->pipe_char_ == '7'  || grid[y][x+1]->pipe_char_ == '-' ){
                    right = true;
                }
            }

            if(up && down){
                this_pipe->pipe_char_ = '|';
            } else if (left && right){
                this_pipe->pipe_char_ = '-';
            } else if (up && right){
                this_pipe->pipe_char_ = 'L';
            } else if (up && left){
                this_pipe->pipe_char_ = 'J';
            } else if (down && right){
                this_pipe->pipe_char_ = 'F';
            } else if (down && left){
                this_pipe->pipe_char_ = '7';
            } else if (left && right){
                this_pipe->pipe_char_ = '-';
            } else {
                std::cout << "Cannot infer S from surroundings." << std::endl;
            }
            assert(this_pipe->pipe_char_ != 'S');
        }

        void connect_network(){
            Pipe * this_pipe = NULL;
            for(size_t y = 0; y < y_; y++){
                for(size_t x = 0; x < x_; x++){
                    this_pipe = grid[y][x];
                    if(this_pipe->pipe_char_ != '.'){
                        //check down
                        if(this_pipe->y_ < y_){
                            if(this_pipe->pipe_char_ == 'F' || this_pipe->pipe_char_ == '7'  || this_pipe->pipe_char_ == '|' ){
                                this_pipe->neighbors_.push_back(grid[y+1][x]);
                            }
                        }
                        //check up
                        if(this_pipe->y_ > 0){
                            if(this_pipe->pipe_char_ == 'L' || this_pipe->pipe_char_ == 'J'  || this_pipe->pipe_char_ == '|' ){
                                this_pipe->neighbors_.push_back(grid[y-1][x]);
                            }
                        }
                        //check left
                        if(this_pipe->x_ < x_){
                            if(this_pipe->pipe_char_ == 'F' || this_pipe->pipe_char_ == 'L'  || this_pipe->pipe_char_ == '-' ){
                                this_pipe->neighbors_.push_back(grid[y][x+1]);
                            }
                        }
                        //check right
                        if(this_pipe->x_ < 0){
                            if(this_pipe->pipe_char_ == 'J' || this_pipe->pipe_char_ == '7'  || this_pipe->pipe_char_ == '-' ){
                                this_pipe->neighbors_.push_back(grid[y][x-1]);
                            }
                        }
                    }
                }
            }
        }




        std::vector<Pipe *> walk_loop(){
            Pipe * this_pipe = start_;
            std::vector<Pipe *> path;
            path.push_back(this_pipe);
            do {
                for(Pipe * neighbor : this_pipe->neighbors_){
                    auto iter = std::find(path.begin(),path.end(),neighbor);
                    if(iter == path.end()){
                        this_pipe = neighbor;
                        path.push_back(this_pipe);
                        break;
                    }
                }
            } while(this_pipe != start_);
            return path;
        }

        std::vector<std::vector<Pipe *>> grid;
        size_t x_ = 0; 
        size_t y_ = 0;
        Pipe * start_ = NULL;
};

int main(){
    //Pipe Maze
    //  Pt1 - find how many steps to get to the farthest part of the loop
    //  Infer shape of pipe on S
    //  Find cycles that begin with S

    
    std::string filename = "10/input_test1.txt";

    PipeGrid * grid = new PipeGrid();
    grid->parse_from_file(filename);
    grid->connect_network();
    std::vector<Pipe *> path = grid->walk_loop();


    std::cout << "The final result for Part 1 is: " << std::ceil(path.size() / 2) << std::endl;
    //std::cout << "The final result for Part 2 is: " << extrapolated_value_sum_rev << std::endl;

    return 0;
}