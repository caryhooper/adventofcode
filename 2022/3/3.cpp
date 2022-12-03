#include <cstdio>
#include <cstdlib>
#include <string>    
#include <ios>
#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>

//Define RuckSack class that keeps track of the sacks' contents, compartments, and items.
//  Also provides methods to calculate priority score and find common items between compartments.
class RuckSack{
    private:
        std::string compartment1;
        std::string compartment2;

        void sortContents(){
            uint64_t num_items = this->contents.length();
            if(num_items % 2 != 0){
                std::cout << "Warning... number of items in rucksack is " << num_items << " (" << contents << ")";
            }
            uint64_t halfway = num_items / 2;
            this->compartment1 = this->contents.substr(0,halfway);
            this->compartment2 = this->contents.substr(halfway,halfway);
        }

    public:
        std::string contents;

        //returns priority score for given char item.
        uint64_t getPriorityFromItem(char item){
            // Lowercase item types a through z have priorities 1 through 26.
            // Uppercase item types A through Z have priorities 27 through 52.
            if(item >= 97 && item <= 122){
                //lowercase
                return item - 96;
            } else if (item >= 65 && item <= 90){
                //uppercase
                return item - 38;
            }
            std::cout << "Warning.  End of getPriorityFromItem has been reached.  Returning 0.\n";
            return 0;
        }

        //Finds teh common item between compartments.  Does so with a frequency map 
        char findCommonItem(){
            std::map<char,int> freq_map;
            //First traverse compartment 1 and populate map.
            for(int i = 0; i < (int) this->compartment1.length(); i++){
                if(freq_map.find(this->compartment1[i]) != freq_map.end()){
                    //char is in map
                    freq_map.insert(std::make_pair(this->compartment1[i],0));
                } else {
                    freq_map[this->compartment1[i]]++;
                }
            }
            //Next traverse compartment 2 and return the first char in common.
            for(int j = 0; j < (int) this->compartment2.length(); j++){
                if(freq_map.find(this->compartment2[j]) != freq_map.end()){
                    return this->compartment2[j];
                }
            }
            std::cout << "Warning.  End of freq_map has been reached.  Returning \\0.\n";
            return '\0';
        }

        //Constructor
        RuckSack(std::string contents){
            this->contents = contents;
            this->sortContents(); //separate into Compartment1 and Compartment2
        };
        //Destructor
        ~RuckSack(){};
};

//Method accepts an argument of a vector of n rucksacks.  It returns the first item found in common between all rucks.
char findCommonBadge(std::vector<RuckSack> sacks){
    //Sieve acts like a state machine.   A letter present in a rucksack will move on to the next state
    //  only if it moved in all previous states.
    std::map<char, int> sieve;
    //Init sieve with all ascii letters.
    for (int i = 65; i <= 122; i++){
        if(i < 97 && i > 90){
            continue;
        }
        sieve.insert(std::make_pair((char) i, 0));
    }

    //Iterate through all sacks
    for(int i = 0; i < (int) sacks.size(); i++){
        
        std::string contents = sacks[i].contents;
        //pilfer through the contents...
        for(int j = 0; j < (int) contents.length(); j++){
            char item = contents[j];
            auto iter = sieve.find(item);
            //Only move to next state for sacks that have moved from previous state(s).  Ensures we're only left with letters contained within all three sacks.
            if(iter->second == i){
                iter->second = i + 1;
            }
        }
    }

    //Returns the first item in sieve that has a value of sacks.size() (present in all sacks).
    for(auto const& iter : sieve){
        int final_state = (int) sacks.size();
        if(iter.second == final_state){
            return iter.first;
        }
    }
    std::cout << "Warning... no badge found for these rucksacks.";
    return '\0';
}

int main(){

    std::string filename = "3/input.txt";
    std::ifstream file_stream(filename,std::ios::in);
    std::string line;
    uint64_t priority_sum = 0;

    uint64_t badge_sum = 0;
    std::vector<RuckSack> ruck_vector;

    //Iterate through input file.
    while(file_stream){
        getline(file_stream,line);
        line.erase(std::remove(line.begin(),line.end(), ' '), line.end());
        if(line.empty()){
            //skip empty lines
            continue;
        }

        //Part1
        RuckSack ruck = RuckSack(line);
        char common_item = ruck.findCommonItem();
        uint64_t priority = ruck.getPriorityFromItem(common_item);
        //std::cout << "Created rucksack with common item " << common_item << " of priority " << priority << ".\n";

        priority_sum += priority;

        //Part2 (save rucks in vector)
        ruck_vector.push_back(ruck);
    }

    //Iterate through rucks, three at a time
    for(int i = 0; i < (int) ruck_vector.size(); i += 3){
        std::vector<RuckSack> sacks{ruck_vector[i], ruck_vector[i+1], ruck_vector[i+2]};
        //Submit to the method to find the common item (badge)
        char badge = findCommonBadge(sacks);
        uint64_t priority = ruck_vector[i].getPriorityFromItem(badge);
        //std::cout << "Badge found: " << badge << " with priority " << priority << ".\n";

        badge_sum += priority;
    }

    std::cout << "The sum of the priorities of each common item is " << priority_sum << " (part1)\n";
    std::cout << "The sum of the priorities of each badge is " << badge_sum << " (part2)\n";

    return 0;
}