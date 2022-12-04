#include <cstdio>
#include <cstdlib>
#include <string>    
#include <ios>
#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>

std::vector<int> rangeToVector(std::string range){
    //Convert string range (1-4) into a vector containing all integers.  No error handling here but needs it in a prod environment
    std::vector<int> cleaning_areas;
    auto iter = range.find("-");
    std::string s_begin_num = range.substr(0,iter);
    range.erase(0,iter + 1);
    //Convert strings to integers.
    int begin_num = std::stoi(s_begin_num);
    int end_num = std::stoi(range);
    for(int i = begin_num; i <= end_num; i++){
        //Add all ints in range (inclusive) to vector.
        cleaning_areas.push_back(i);
    }
    return cleaning_areas;
}

int main(){

    //First create vectors of cleaning areas. C++ sets suck.  This is to allow us to utilize set_intersection()

    //Init values / open file
    std::string filename = "4/input.txt";
    std::ifstream file_stream(filename,std::ios::in);
    std::string line; 
    uint64_t containment_pairs = 0;
    uint64_t overlapping_pairs = 0;

    while(file_stream){
        //Iterate through lines in file.
        getline(file_stream,line);
        line.erase(std::remove(line.begin(),line.end(), ' '), line.end());
        if(line.empty()){
            //skip empty lines
            continue;
        }

        //Split the string into two ranges.
        //C++ split a string https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
        //2-4,6-8
        auto iter = line.find(",");
        std::string elf1 = line.substr(0,iter);
        //Pass string range with "-" into rangeToVector function
        std::vector<int> range1 = rangeToVector(elf1);
        line.erase(0,iter + 1);
        std::vector<int> range2 = rangeToVector(line);

        //Find the size of each range.  
        size_t smallest_set = (range1.size() < range2.size())? range1.size() : range2.size();

        //No need to sort because we're already sorted.

        //Define vector to store our intersecting values
        std::vector<int> intersection;
        //Reserve space within our vector in case all values overlap
        intersection.reserve(smallest_set);
        std::set_intersection(range1.begin(), range1.end(), range2.begin(), range2.end(),std::back_inserter(intersection));

        //Increment the outcome values to get part1 and part2 answers.
        //std::cout << "The size of the intersection of each set is " << intersection.size() << ".\n";
        
        if(intersection.size() == smallest_set){
            //Part1
            containment_pairs++;
        }
        if(intersection.size() > 0){
            //Part2
            overlapping_pairs++;
        }
    }

    //Part 1
    std::cout << "The number of pairs where one assignment fully contains the other is " << containment_pairs << " (part1).\n";

    //Part 2
    std::cout << "The number of pairs that has at least one overlapping assignment is " << overlapping_pairs << " (part2).\n";
    return 0;
}