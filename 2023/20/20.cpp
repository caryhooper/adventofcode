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

class Sequence{
    public:
        Sequence(std::string line){
            while(!line.empty()){
                std::string num;
                num = parse_until_delim(line);
                int num_int = std::stoi(num);
                sequence.push_back(num_int);
            }
        }

        int extrapolate_backward(std::vector<int> seq){
            if(std::accumulate(seq.begin(),seq.end(),0) == 0){
                //std::cout << "Identified all 0s" << std::endl;
                return 0;
            }

            std::vector<int> next_seq;
            for(int i = 0; i < (int) seq.size() - 1; i++){
                next_seq.push_back(seq[i+1] - seq[i]);
            }
            assert(next_seq.size() == seq.size() - 1);

            int next_inc = this->extrapolate_backward(next_seq);
            int last_val = seq.front();
            return last_val - next_inc;
        }

        int extrapolate_forward(std::vector<int> seq){
            if(std::accumulate(seq.begin(),seq.end(),0) == 0){
                //std::cout << "Identified all 0s" << std::endl;
                return 0;
            }

            std::vector<int> next_seq;
            for(int i = 0; i < (int) seq.size() - 1; i++){
                next_seq.push_back(seq[i+1] - seq[i]);
            }
            assert(next_seq.size() == seq.size() - 1);

            int next_inc = this->extrapolate_forward(next_seq);
            int last_val = seq.back();
            return last_val + next_inc;

        }
        std::vector<int> sequence;
};

int main(){
    //Mirage Maintenance
    //  each line history of a single value
    //  env report should have prediction of next value
    //  new sequence = difference at each step.  If not all zeros, repea
    size_t extrapolated_value_sum = 0;
    size_t extrapolated_value_sum_rev = 0;

    
    std::string filename = "9/input.txt";

    std::ifstream input_file(filename);
    std::string line; 
    Sequence * seq = NULL;
    int next_val = 0;
    while(std::getline(input_file,line)){
        seq = new Sequence(line);
        next_val = seq->extrapolate_forward(seq->sequence);
        std::cout << "The next value is " << next_val << std::endl;
        extrapolated_value_sum += next_val;

        next_val = seq->extrapolate_backward(seq->sequence);
        extrapolated_value_sum_rev += next_val;
    }

    std::cout << "The final result for Part 1 is: " << extrapolated_value_sum << std::endl;
    std::cout << "The final result for Part 2 is: " << extrapolated_value_sum_rev << std::endl;





    return 0;
}