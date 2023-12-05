#include <string> 
#include <iostream> //
#include <algorithm> //reverse
#include <fstream> //ifstream
#include <map> //map
#include <set>
#include <math.h>
#include <cassert>
#include <map>
#include <limits>
#include "../helpers.hpp"

using namespace advent_of_code;
// std::string parse_until_delim(std::string& input, char c = ' ');
// void trim(std::string& input, char c = ' ');
// size_t strtoul(std::string result);

class NumMap{
    public:
        NumMap(size_t dst, size_t src, size_t len){
            dst_ = dst;
            src_min_ = src;
            len_ = len;
            src_max_ = src_min_ + len_ - 1;
        }
        bool is_in_range(size_t query_num){
            if(query_num >= src_min_ && query_num <= src_max_){
                return true;
            } else {
                return false;
            }
        }
        size_t lookup(size_t query_num){
            assert(is_in_range(query_num));
            size_t delta = query_num - src_min_;
            return dst_ + delta;
        }

        size_t dst_;
        size_t src_min_;
        size_t src_max_; 
        size_t len_;
};

class Almanac{
    public:
        Almanac(){
            //https://stackoverflow.com/questions/25108854/initializing-the-size-of-a-c-vector
            std::vector<NumMap *> this_map;
            // lookup_table_.reserve(100);
            // for(int i = 0; i < 7; i++){
            //     lookup_table_.push_back(this_map);
            // }
            // lookup_table_.shrink_to_fit(); //just trying this out for funzies

            std::fill_n(std::back_inserter(lookup_table_),7,this_map);
        }
        std::vector<NumMap *> parse_from_file(std::string filename){
            std::ifstream input_file(filename);
            std::string line; 
            std::getline(input_file, line);
            std::vector<NumMap *> seeds;
            assert(line.substr(0,6).compare("seeds:") == 0);
            parse_until_delim(line,' '); //now all that is left is seeds. 
            std::string result;
            std::string seed_start;
            std::string seed_range;
            while(!line.empty()){
                seed_start = parse_until_delim(line,' ');
                seed_range = parse_until_delim(line,' ');
                seeds.push_back(new NumMap((size_t) 0,strtoul(seed_start),strtoul(seed_range)));
            }

            int state = -1;
            std::string first;
            std::string second;
            std::string third;

            while(std::getline(input_file,line)){
                if(line.empty()){
                    state += 1;
                    std::getline(input_file, line);// skip first line bc line is empty
                    //assert we're working on a map now
                    std::string map_str_probably = line.substr(line.size() - 4,4);
                    assert(map_str_probably.compare("map:") == 0);
                    std::vector<NumMap *> this_map;
                    lookup_table_[state] = this_map;
                } else {
                    first = parse_until_delim(line);
                    second = parse_until_delim(line);
                    third = parse_until_delim(line);

                    NumMap * this_nummap = new NumMap(strtoul(first),strtoul(second),strtoul(third));
                    lookup_table_[state].push_back(this_nummap);
                    //std::cout << "foo" <<std::endl;
                }

            }
            return seeds;

        }
        
        size_t lookup(size_t seed){
            int state = -1;
            size_t result = seed;
            for(std::vector<NumMap *> row : lookup_table_){
                state += 1;
                //std::cout << " [" << state << "] " << result << ",";

                //bool is_in_row = false;
                for(NumMap * nm : row){
                    if(nm->is_in_range(result)){
                        result = nm->lookup(result);
                        break;
                    }
                }
                //we get here we conclude that lookup is not in any range and it is equal to itself.
            
            }
            //std::cout << " [" << state << "] " << result << std::endl;
            return result;
        }
        std::vector<std::vector<NumMap *>> lookup_table_;
        // std::vector<NumMap> seed_to_soil;
        // std::vector<NumMap> soil_to_fertilizer;
        // std::vector<NumMap> fertilizer_to_water;
        // std::vector<NumMap> water_to_light;
        // std::vector<NumMap> light_to_temperature;
        // std::vector<NumMap> temperature_to_humidity;
        // std::vector<NumMap> humidity_to_location;
};

// seeds: 79 14 55 13

// seed-to-soil map:
// 50 98 2
// 52 50 48

// soil-to-fertilizer map:
// 0 15 37
// 37 52 2
// 39 0 15

// fertilizer-to-water map:
// 49 53 8
// 0 11 42
// 42 0 7
// 57 7 4

// water-to-light map:
// 88 18 7
// 18 25 70

// light-to-temperature map:
// 45 77 23
// 81 45 19
// 68 64 13

// temperature-to-humidity map:
// 0 69 1
// 1 0 69

// humidity-to-location map:
// 60 56 37
// 56 93 4

int main(){
    //Scratchcards
    size_t lowest_location = UINT32_MAX;

    std::string filename = "5/input.txt";

    Almanac * a = new Almanac();
    std::vector<NumMap *> seeds = a->parse_from_file(filename);
    
    size_t location = 0;
    for(NumMap * seed : seeds){
        std::cout << "Analyzing seed_start: " << seed->src_min_ << std::endl;
        for(size_t seed_no = seed->src_min_; seed_no < seed->src_max_; seed_no++){
            location = a->lookup(seed_no);
            if(location < lowest_location){
                lowest_location = location;
            }
        }
    }



    std::cout << "The final result for Part 2 is: " << lowest_location << std::endl;


    //cleanup
    for(std::vector<NumMap *> map : a->lookup_table_){
        for(NumMap * nm : map){
            delete nm;
            nm = NULL;
        }
    }
    delete a;
    a = NULL;
    for(NumMap * seed : seeds){
        delete seed;
        seed = NULL;
    }

    return 0;
}