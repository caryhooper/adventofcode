#include <cstdio>
#include <cstdlib>
#include <string>    
#include <ios>
#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>

//tasks.json
// {
//     "version":"1.0.0",
//     "tasks":[
//         {
//             "label": "build",
//             "type": "shell",
//             "command": "/usr/bin/g++",
//             "group":"build",
//             "args": ["-g3","-Wall","-Werror",/*"-fsanitize=address",*/"-fno-omit-frame-pointer","-Wno-format-security","day1.cpp","-o","part1.elf","&&","chmod","+x","part1.elf"]
//         }
//     ]
// }

//Define backpack class to keep track of the number of items/calories each elf has on their back.
class Elf_backpack{
    public:
        uint64_t total_calories;
        std::vector<uint64_t> items;

        //Constructor
        Elf_backpack(){
            this->total_calories = 0;
        };
        //Destructor
        ~Elf_backpack(){};

        void add_item(uint64_t cal){
            this->total_calories += cal;
            this->items.push_back(cal);
        }
};

// //Count number of lines in the file.
// uint64_t numLinesInFile(std::string filename){
//     uint64_t nl_count = 0;
//     std::string line;
//     std::ifstream f(filename,std::ios::in);
//     while(f){
//         getline(f,line);
//         nl_count++;
//     }
//     return nl_count;
// }

int main(){
    //Create vector of Elf_backpack objects
    std::vector<Elf_backpack> backpacks;

    //Open file as an ifstream
    std::string filename = "input.txt";
    std::ifstream f(filename,std::ios::in);

    //Initialize variables to walk through file
    std::string line;
    uint64_t cals = 0;
    bool create_new_backpack = true;

    //Iterate through each line
    while(f){
        getline(f,line);
        //If the line is empty set the flag
        if(line.empty()){
            create_new_backpack = true;
        } else {
            //Create new backpack for filling with calories/items
            if(create_new_backpack){
                Elf_backpack bp = Elf_backpack();
                backpacks.push_back(bp);
                create_new_backpack = false;
            }
            //Convert string to integer and assign to cals.
            cals = std::stoi(line);
            //Add this item to the current elf's pack (backpack at end of vector is current one)
            backpacks.back().add_item(cals);
        }        
    }
    //file ingested into data "backpacks" data structure
 
    //uint64_t most_calories = 0; //part1
    std::vector<uint64_t> calorie_counts; //part2

    //push all total_calories into a vector
    for(uint64_t i = 0; i < backpacks.size(); i++){
        //if(backpacks[i].total_calories > most_calories){  //part1
        //  most_calories = backpacks[i].total_calories;  //part1
        //}  //part1
        calorie_counts.push_back(backpacks[i].total_calories);
    }

    //sort the vector
    sort(calorie_counts.begin(), calorie_counts.end());

    //Slice the vector to get the top three
    std::vector<uint64_t> topthree(calorie_counts.end() - 3, calorie_counts.end());
    uint64_t top_three_total = 0;
    for(size_t i = 0; i < topthree.size(); i++){
        top_three_total += topthree[i];
    }


    //std::cout << "The elf with the most calories has " << most_calories; //part1
    std::cout << "The top three elves are carrying " << top_three_total << " calories.\n";

    //Alternately, we can use std::accumulate
    std::cout << "We get the same answer with std::accumulate. " << std::accumulate(topthree.begin(),topthree.end(),decltype(topthree)::value_type(0)) << "\n";

    return 0;
}