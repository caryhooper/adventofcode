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

class CCHand{
    public:
        CCHand(std::string input_line){
            parse_hand(input_line);
            strength = determine_strength();
            upgrade_hand();
        }
        std::vector<char> hand;
        std::string hand_str;
        size_t bid;
        size_t joker_count = 0;

        enum Strength{
            highcard,
            onepair,
            twopair,
            threekind,
            fullhouse,
            fourkind,
            fivekind
        };
        Strength strength; 
    private:

        void upgrade_hand(){
            //for each joker, 0-->1, 1-->3, 2-->4, 3-->5, 5-->6
            for(size_t i = 0; i < joker_count; i++){
                if(strength == fivekind){
                    return;
                }
                if(strength == highcard || strength == fourkind){
                    strength = static_cast<Strength>(static_cast<int>(strength) + 1);
                } else {
                    strength = static_cast<Strength>(static_cast<int>(strength) + 2);
                }

            }
            if(static_cast<int>(strength) > 6){
                strength = fivekind;
            }
        }

        Strength determine_strength(){
            // std::vector<int> card_count(13,0);
            // for(char c: hand){
            //     card_count[c]+=1;
            // }
            //std::vector<char> pair_char;
            size_t num_pairs = 0; //Note.  num_pairs = 0... high card.  
            //                                        =1... onepair
            //                                        =2... twopair 
            //                                        =3... threekind
            //                                        =4... fullhouse
            //                                        =6... fourkind (5)
            //                                        =10... fivekind (6)
            for(int i = 0; i < 5; i++){
                if(hand[i] == 1){
                    joker_count = joker_count + 1;
                    continue;
                }
                for(int j = i; j < 5; j++){
                    //Check for joker

                    if(i != j && hand[i] == hand[j]){
                        num_pairs += 1;
                        //pair_char.push_back(hand[i]);
                    }
                }
            }

            //upgrade hand for each joker.

            switch(num_pairs){
                case 0:
                    return highcard;
                case 1:
                    return onepair;
                case 2:
                    // if(pair_char[0] == pair_char[1]){
                    //     return threekind;
                    // } else {
                        return twopair;
                    //}
                case 3:
                    return threekind;
                case 4:
                    return fullhouse;
                case 6:
                    return fourkind;
                case 10:
                    return fivekind;
                default:
                    std::cout << "Error: num_pairs calculated for " << this->hand_str << " num_pairs=" << num_pairs << std::endl;
                    exit(0);
            }
        }
        void parse_hand(std::string line){
            std::string token = parse_until_delim(line);
            trim(token);
            hand_str = token;
            for(char c : token){
                hand.push_back(convert_card_to_value(c));
            }
            token = parse_until_delim(line);
            trim(token);
            bid = strtoul(token);
        }
        char convert_card_to_value(char c){
            //0 (48) || 2 (50),3,4,5,6,7,8,9 (57) stay same
            if(c > 49 && c < 58){
                return c - 48;
            }
            //T(84)-->58, J(74)-->59, Q (81)-->60, K(75)-->61, A(65)-->62
            switch(c){
                case 84:
                    return 58-48;
                case 74:
                    //Jokers are now 1 return 59-48;
                    return 1;
                case 81:
                    return 60-48;
                case 75:
                    return 61-48;
                case 65:
                    return 62-48;
                default:
                    std::cout << "Char not recognized: " << c << std::endl;
                    return '\0';
            }
        }
};

bool compare_cchands(CCHand * h1, CCHand * h2){ //is h1 greater than h2?
    if(h1->strength > h2->strength){
        return false;
    } else if (h1->strength <  h2->strength){
        return true;
    } else {
        //equal
        for(int i = 0; i < 5; i++){
            if(h1->hand[i] > h2->hand[i]){
                return false;
            }
            if (h1->hand[i] < h2->hand[i]){
                return true;
            }   
        }
    }
    return true; //aribtrary (they are equal)
}


int main(){
    //Camel Cards
    //  TODO - process each hand, determine the strength
    //  TODO - compare hands 
    //  TODO - sort hands
    //  TODO - calculate sum of the rank (1 is lowest) 
    //Plan is to store all CCHand objects in a vector and use std::sort to sort them.  
    size_t winnings  = 0;
    std::string filename = "7/input.txt";

    std::ifstream input_file(filename);
    std::string line; 
    std::vector<CCHand *> hands;
    CCHand * hand = NULL;
    while(std::getline(input_file,line)){
        if(!line.empty()){
            hand = new CCHand(line);
            hands.push_back(hand);
        }
    }

    std::sort(hands.begin(), hands.end(), compare_cchands);

    //calculate winnings
    size_t rank = 0;
    size_t winning = 0;
    for(int i = 0; i < (int) hands.size(); i++){
        rank = i+1;
        hand = hands[i];
        winning = rank * hand->bid;
        winnings = winnings + winning;
        std::cout << "The winnings for " << hand->hand_str << " is " << winning << " (rank=" << rank << " bid=" << hand->bid << ")" << std::endl;
    }


    std::cout << "The final result for Part 1 is: " << winnings << std::endl;

    //cleanup
    // for(CCHand * this_hand: hands){
    //     delete this_hand;
    //     this_hand = NULL;
    // }

    return 0;
}