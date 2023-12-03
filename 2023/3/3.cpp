#include <string> 
#include <iostream> //
#include <algorithm> //reverse
#include <fstream> //ifstream
#include <map> //map
#include <set>
#include <math.h>
#include <cassert>
#include <map>

class Gear{
    public:
        Gear(std::pair<size_t,size_t> coord){
            coord_ = coord;
        }
        void add_part_id(size_t part_id){
            part_ids_.push_back(part_id);
        }
        size_t get_gear_ratio(){
            if(part_ids_.size() == 2){
                return part_ids_[0]*part_ids_[1];
            } else {
                return 0;
            }
        }
        std::pair<size_t,size_t> coord_;
        std::vector<size_t> part_ids_;
};

class Part{
    public:
        Part(size_t id,std::vector<std::pair<size_t,size_t>> coords){
            id_ = id;
            for(std::pair<size_t,size_t> coord : coords){
                coords_.push_back(coord);
            }
        }
        void add_symbol(std::pair<size_t,size_t> coord, char c){
            symbols_.insert(std::pair<std::pair<size_t,size_t>,char>(coord,c));
        }
        size_t id_;
        std::vector<std::pair<size_t,size_t>> coords_;
        std::map<std::pair<size_t,size_t>,char> symbols_;
};

class charGrid{
    public:
        charGrid(){

        }
        void calc_x(){
            //x is the largest sub-vector
            x_ = 0; 
            for(std::vector<char> row: grid){
                size_t size_row = row.size();
                if(size_row  > x_){
                    x_ = size_row;
                }
            }
        }
        void calc_y(){
            y_ = grid.size();
        }

        bool is_digit(char c){
            if(c > 47 && c < 58){
                return true;
            }
            return false;
        }
        bool is_digit(size_t x, size_t y){
            char c = this->grid[y][x];
            if(c > 47 && c < 58){
                return true;
            }
            return false;
        }
        bool is_digit(std::pair<size_t,size_t> this_pair){
            char c = this->grid[this_pair.second][this_pair.first];
            if(c > 47 && c < 58){
                return true;
            }
            return false;
        }

        //symbol does not include number or period
        bool is_symbol(char c){
            if(c > 20 && c < 48){
                if(c != 46){
                    return true;
                } else {
                    return false;
                }
            }
            if(c > 57 && c < 65){
                return true;
            }
            if(c > 90 && c < 97){
                return true;
            }
            if(c > 122 && c < 127){
                return true;
            }
            return false;
        }
        bool is_symbol(size_t x, size_t y){
            char c = this->grid[y][x];
            if(c > 20 && c < 48){
                if(c != 46){
                    return true;
                } else {
                    return false;
                }
            }
            if(c > 57 && c < 65){
                return true;
            }
            if(c > 90 && c < 97){
                return true;
            }
            if(c > 122 && c < 127){
                return true;
            }
            return false;
        }
        bool is_symbol(std::pair<size_t,size_t> this_pair){
            char c = this->grid[this_pair.second][this_pair.first];
            if(c > 20 && c < 48){
                if(c != 46){
                    return true;
                } else {
                    return false;
                }
            }
            if(c > 57 && c < 65){
                return true;
            }
            if(c > 90 && c < 97){
                return true;
            }
            if(c > 122 && c < 127){
                return true;
            }
            return false;
        }
        
        std::vector<std::pair<size_t,size_t>> get_square(size_t x, size_t y){
            std::vector<size_t> possible_y;
            possible_y.push_back(y);
            if(y != y_-1){
                possible_y.push_back(y+1);
            }
            if(y != 0){
                possible_y.push_back(y-1);
            }

            std::vector<size_t> possible_x;
            possible_x.push_back(x);
            if(x != x_-1){
                possible_x.push_back(x+1);
            }
            if(x != 0){
                possible_x.push_back(x-1);
            }
            std::vector<std::pair<size_t,size_t>> pairs;
            for(size_t this_x : possible_x){
                for(size_t this_y : possible_y){
                    pairs.push_back(std::pair<size_t,size_t>(this_x,this_y));
                }
            }
            return pairs;
        }

        //Includes diagonals
        std::vector<std::pair<size_t,size_t>>  find_adjacent_cells(size_t x, size_t y){
            std::vector<std::pair<size_t,size_t>> adjacents = get_square(x,y);
            auto iter = std::find(adjacents.begin(),adjacents.end(),std::pair<size_t,size_t>(x,y));
            adjacents.erase(iter);
            return adjacents;
        }

        //Includes diagonals
        bool is_adjacent_to_symbol( std::vector<std::pair<size_t,size_t>> number_cells){
            bool is_part = false;
            std::set<std::pair<size_t,size_t>> adjacent_set;
            for(std::pair<size_t,size_t> this_coord: number_cells){
                //iterate through numbers found
                for(std::pair<size_t,size_t> this_adj : get_square(this_coord.first,this_coord.second)){
                    //iterate through all adjacent to number
                    adjacent_set.insert(this_adj);
                }
            }
            //next we remove all items in number_cells
            for(std::pair<size_t,size_t> this_coord: number_cells){
                auto iter = std::find(adjacent_set.begin(), adjacent_set.end(),this_coord);
                if(iter != adjacent_set.end()){
                    adjacent_set.erase(iter);
                }
            }
            //finally we determine if any in adjacent_set is a symbol.
            for(std::pair<size_t,size_t> this_adj : adjacent_set){
                if(is_symbol(this_adj)){
                    //deal with parts
                    char symbol_char = this->grid[this_adj.second][this_adj.first];
                    size_t part_id = get_part_number_from_vector(number_cells);
                    //lambdas! //https://stackoverflow.com/questions/15517991/search-a-vector-of-objects-by-object-attribute
                    auto it = std::find_if(parts_.begin(), parts_.end(), [&part_id](Part* part){return part->id_ == part_id;});
                    if(it == parts_.end()){
                        Part * this_part = new Part(part_id,number_cells);
                        this_part->add_symbol(this_adj,symbol_char);
                        parts_.push_back(this_part);
                    } else {
                       (*it)->add_symbol(this_adj,symbol_char);
                    }
                    //deal with gears
                    if(symbol_char == 42){
                        auto it2 = std::find_if(gears_.begin(), gears_.end(), [&this_adj](Gear * gear){return gear->coord_ == this_adj;});
                        if(it2 == gears_.end()){
                            //gear not found
                            Gear * gear = new Gear(this_adj);
                            gear->add_part_id(part_id);
                            gears_.push_back(gear);
                        } else {
                            //gear found
                            (*it2)->add_part_id(part_id);
                        }
                    }

                    is_part = true;
                }
            }
            return is_part;
        }

        bool is_adjacent_to_symbol(std::pair<size_t,size_t> this_pair){
            for(std::pair<size_t,size_t> adj_pair :  find_adjacent_cells(this_pair.first, this_pair.second)){   
                if(is_symbol(grid[adj_pair.second][adj_pair.first])){
                    return true;
                }
            }
            return false;
        }

        size_t get_part_number_from_vector(std::vector<std::pair<size_t,size_t>> part_vector){
            size_t sum = 0;
            char this_c = 0;
            size_t power = 0;
            for(size_t i = 0; i < part_vector.size(); i++){
                this_c = this->grid[part_vector[i].second][part_vector[i].first];
                power = part_vector.size() -i -1;
                assert(is_digit(part_vector[i]));
                sum += ((size_t) pow((double) 10,(double) power))*((size_t)(this_c - 48));
            }
            std::cout << "Returning number: " << sum << std::endl;
            return sum;
        }



        void parse_from_file(std::string filename){
            std::ifstream input_file(filename);
            std::string line;
            while(std::getline(input_file,line)){
                std::vector<char> row;
                for(char c : line){
                    row.push_back(c);
                }
                grid.push_back(row);
            }
            calc_x();
            calc_y();
        }

        size_t sum_gear_ratios(){
            size_t sum = 0;
            for(Gear * gear : gears_){
                sum += gear->get_gear_ratio();
                delete gear;
                gear = NULL;
            }
            return sum;
        }

        std::vector<std::vector<char>> grid;
        size_t x_ = 0; 
        size_t y_ = 0;
        std::vector<Part *> parts_;
        std::vector<Gear *> gears_;
};

//Gear Ratios
int main(){
    //Engine Schematic
    size_t total = 0;
    charGrid schematic = charGrid();
    schematic.parse_from_file("3/input.txt");

    // std::vector<std::pair<size_t,size_t>> search_space;
    // for(size_t x = 0; x < schematic.x_; x++){
    //     for(size_t y = 0; y < schematic.y_; y++){
    //         search_space.push_back(std::pair(x,y));
    //     }
    // }
    //Iterate through grid, identify related numbers and group them together.
    std::vector<std::pair<size_t,size_t>> contiguous_numbers;
    for(size_t y = 0; y < schematic.y_; y++){
        for(size_t x = 0; x < schematic.x_; x++){
            if(schematic.is_digit(x,y)){
                contiguous_numbers.push_back(std::pair(x,y));
                while(schematic.is_digit(x+1,y) && x+1 < schematic.x_){
                    contiguous_numbers.push_back(std::pair(x+1,y));
                    x += 1;
                }
                //now we have all coords loaded into contiguous_numbers vector.  Need to check that at least one adjacent number is a symbol
                if(schematic.is_adjacent_to_symbol(contiguous_numbers)){
                    size_t part_number = schematic.get_part_number_from_vector(contiguous_numbers);
                    total += part_number;
                }
            }
            contiguous_numbers.clear();

        }
    }

    size_t gear_ratio_result = schematic.sum_gear_ratios();

    std::cout << "The final result for Part 1 is: " << total << std::endl;
    std::cout << "The final result for Part 2 is: " << gear_ratio_result << std::endl;

    return 0;
}