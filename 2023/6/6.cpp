#include <string> 
#include <iostream> //
#include <algorithm> //reverse
#include <fstream> //ifstream
#include <map> //map
#include <set>
#include <cmath>
#include <cassert>
#include <map>
#include <limits>
#include "../helpers.hpp"

using namespace advent_of_code;

typedef struct race_t {
    long double time;
    long double dist;
} race_t;

class BoatRace {
    public:
        BoatRace(race_t race_params){
            time_ = race_params.time;
            record_dist_ = race_params.dist;
        }
        long double time_ = 0; //ms
        long double record_dist_ = 0;
        long double lower_presstime_;
        long double upper_presstime_;

        // double calc_dist(double x){
        //     double retval =  (-1 * x * x) + ((double) time_ * x) + (-1* (double) record_dist_);
        //     return retval;
        // }

        void calc_roots(){
            long double x1,x2,a,b,c = 0;
            a = -1;
            b = time_;
            c = -1 * record_dist_;
            assert(time_ != 0);
            //https://stackoverflow.com/questions/898076/solve-quadratic-equation-in-c
            x1 = ((-1*b) + (sqrt((b*b)-(4*a*c)))) / (2*a);
            lower_presstime_ = std::ceil(x1);
            if(x1 == lower_presstime_){
                lower_presstime_ += 1;
            }

            x2 = ((-1*b) - (sqrt((b*b)-(4*a*c)))) / (2*a);
            upper_presstime_ = std::floor(x2);
            if(x2 ==  upper_presstime_){
                upper_presstime_ -= 1;
            }
        }
        size_t get_winning_combos(){
            return upper_presstime_ - lower_presstime_ + 1;
        }
};

int main(){
    //Wait For It
    //Input: Time allowed for each race + record distance (need to beat)
    //   hold button --> charge (longer = faster)  1ms=1mm/ms
    //   release button --> move

    //TODO - find number of integers for hold time resulting in beating the record for each race. Multiply these
    long double product = 1;
    std::vector<race_t *> races;
    std::string part2_time;
    std::string part2_dist;

    std::string filename = "6/input.txt";

    std::ifstream input_file(filename);
    std::string line; 
    std::string token;
    std::getline(input_file,line);
    assert(line.substr(0,5).compare("Time:") == 0);
    parse_until_delim(line);
    long double time = 0;
    long double distance = 0;
    race_t * race;
    while(!line.empty()){
        token = parse_until_delim(line);
        trim(token);
        if(token.empty()){
            continue;
        }
        time = strtoul(token);
        race = new race_t();
        race->time = time;
        part2_time.append(token);
        races.push_back(race);
    }
    std::getline(input_file,line);
    assert(line.substr(0,9).compare("Distance:") == 0);
    parse_until_delim(line);
    int counter = 0;

    while(!line.empty()){
            token = parse_until_delim(line);
            trim(token);
            if(token.empty()){
                continue;
            }
            distance = strtoul(token);
            part2_dist.append(token);
            races[counter]->dist = distance;
            counter += 1;
    }

    //Count it all up
    BoatRace * br;
    long double combos;
    for(race_t * race : races){
        br = new BoatRace(*race);
        br->calc_roots();
        combos = br->get_winning_combos();
        std::cout << "Race combos is " <<  combos << std::endl;
        product = product * combos;
        delete br;
        br = NULL;
    }

    race_t params;
    params.time = strtoul(part2_time);
    params.dist = strtoul(part2_dist);
    BoatRace p2race(params);
    p2race.calc_roots();
    long double part2solution = p2race.get_winning_combos();

    std::cout << "The final result for Part 1 is: " << product << std::endl;
    std::cout << "The final result for Part 2 is: ";
    printf("%Lf\n",part2solution);


    return 0;
}