#include <string> 
#include <iostream> //
#include <algorithm> //reverse
#include <fstream> //ifstream
#include <map> //map

std::map<std::string,size_t> word_dict = {
    {"one"  , 1},
    {"two"  , 2},
    {"three", 3},
    {"four" , 4},
    {"five" , 5},
    {"six"  , 6},
    {"seven", 7},
    {"eight", 8},
    {"nine" , 9}
};

size_t get_calibration_value(std::string line){
    size_t result = 0;
    //std::string line_rev = line;
    //std::reverse(line_rev.begin(),line_rev.end());

    //forward
    size_t j = 0;
    size_t first_num = 0;
    size_t second_num = 0;
    for(j = 0; j < line.size(); j++){
        if(line[j] >= 47 && line[j] <= 57){
            first_num = (line[j]-48);
            j = line.size();
        } 
        else {
            //loop through map and check if substr is in str
            int len = 0;
            for(const auto& [num_string, num] : word_dict){
                len = num_string.size();
                if(j+len <= line.size()){
                    if(num_string.compare(line.substr(j,len)) == 0){
                        auto it = word_dict.find(num_string);
                        first_num = (it->second);
                        j = line.size();
                    }
                }
            }
        }
    }
    //reverse-in-place
    for(size_t i = line.size() - 1; i > 0; --i){
        if(line[i] >= 47 && line[i] <= 57){
            second_num = (line[i]-48);
            i =1;
        } else {
            size_t len = 0;
            for(const auto& [num_string, num] : word_dict){
                len = num_string.size();
                if(i + len <= line.size()){
                    if(num_string.compare(line.substr(i,len)) == 0){
                        auto it = word_dict.find(num_string);
                        second_num = (it->second);
                        i = 1;
                        break;
                    }
                }

            }
        }
    }
    if(second_num == 0){
       result = first_num*11;
    } else {
        result = (first_num*10) + second_num;
    }
    return result;
}

//Global snow production mission.
int main(){
    //calibration value found by combining the first digit and the last digit --> 2 digit number

    //Goal - calculate sum of calibration values.
    std::ifstream input_file("1/input.txt");
    std::string line;
    int total = 0;
    int result = 0;
    while(std::getline(input_file,line)){
        result = get_calibration_value(line);
        std::cout << "Calibration value for " << line << " is: " << result <<std::endl;
        total += result;
    }
    std::cout << "The final result is: " << total << std::endl;
    return 0;
}