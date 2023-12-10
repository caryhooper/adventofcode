#include <string> 
#include <iostream> //
#include <algorithm> //reverse
#include <fstream> //ifstream
#include <map> //map
#include <set>
#include <math.h>
#include <cassert>
#include <map>

namespace advent_of_code
{
    void trim_r(std::string& input, char c = ' '){
        while(input[input.size() - 1] == ' '){
            input.erase(input.size()-1,std::string::npos);
        }
    }
    void trim_l(std::string& input, char c = ' '){
        while(input[0] == ' '){
            input.erase(0,1);
        }
    }
    void trim(std::string& input, char c = ' '){
        trim_l(input, c);
        trim_r(input, c);
    }
    std::string parse_until_delim(std::string& input, char c = ' '){
        //Note: need to remove delim here
        auto iter = input.find(c);
        std::string result;
        if(iter == std::string::npos){
            //no char found.  return entire string
            result = input;
            input.clear();
            return result;
        }
        result = input.substr(0,iter);
        input.erase(0,iter+1);
        //Note need to handle no delim found in string.
        return result;
    }
    int find_sizet_in_sorted_vector(size_t needle, std::vector<size_t> haystack){
        int max = (int) haystack.size();
        for(int i = 0; i < max; i++){
            if(needle == haystack[i]){
                return i;
            } else if (needle < haystack[i]){
                return -1;
            } else {
                continue;
            }
        }
        return -1;
    }

    size_t strtoul(std::string result){
        return std::strtoul(result.c_str(), NULL, 10);
    }

    class Helpers{

    };
}