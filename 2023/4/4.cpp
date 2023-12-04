#include <string> 
#include <iostream> //
#include <algorithm> //reverse
#include <fstream> //ifstream
#include <map> //map
#include <set>
#include <math.h>
#include <cassert>
#include <map>


//helper functions

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




class Scratchcard{
    public:
        Scratchcard(std::string line){
            parse_card(line);
        }
        void parse_card(std::string line){
            //parse card_id
            size_t num = 0;
            std::string result = ::parse_until_delim(line,' ');
            assert(result.compare("Card") == 0);
            result = ::parse_until_delim(line,':');
            card_id_ = std::strtoul(result.c_str(),NULL,10);
            std::string winners =  ::parse_until_delim(line,'|');
            ::trim(winners);
            while(!winners.empty()){
                result = ::parse_until_delim(winners,' ');
                if(!result.empty()){
                    ::trim(result);
                    num = std::strtoul(result.c_str(),NULL,10);
                    winning_numbers_.push_back(num);
                }
            }
            std::string card_numbers = ::parse_until_delim(line,'|');
            while(!card_numbers.empty()){
                result = ::parse_until_delim(card_numbers,' ');
                if(!result.empty()){
                    ::trim(result);
                    num = std::strtoul(result.c_str(),NULL,10);
                    card_numbers_.push_back(num);
                }

            }

            std::sort(winning_numbers_.begin(), winning_numbers_.end());
            std::sort(card_numbers_.begin(), card_numbers_.end());
        }
        size_t get_points(){
            //find matches, calculate points
            if(!calculated_points_){
                calculated_points_ = true;
                for(size_t winner : winning_numbers_){
                    if(find_sizet_in_sorted_vector(winner, card_numbers_) >= 0){
                        winners_.push_back(winner);
                    }
                }
                size_t num_winners = winners_.size();
                if(num_winners > 0){
                    points_ = (size_t) pow(2,num_winners-1);
                }
            }
            std::cout << "Card " << card_id_ << " is worth " << points_ << " points." << std::endl;
            return points_;
        }
        
        void recurse_cards(size_t& total){
            for(Scratchcard * card: card_copies_){
                total += 1;
                card->recurse_cards(total);
            }
        }
        bool calculated_points_ = false;
        size_t points_ = 0;
        size_t card_id_;
        std::vector<size_t> winning_numbers_;
        std::vector<size_t> winners_; 
        std::vector<size_t> card_numbers_;
        std::vector<Scratchcard *> card_copies_;
};

// Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
// Card 2: 13 32 20 16 61 | 61 30 68 82 17 32 24 19
// Card 3:  1 21 53 59 44 | 69 82 63 72 16 21 14  1
// Card 4: 41 92 73 84 69 | 59 84 76 51 58  5 54 83
// Card 5: 87 83 26 28 32 | 88 30 70 12 93 22 82 36
// Card 6: 31 18 13 56 72 | 74 77 10 23 35 67 36 11

int main(){
    //Scratchcards
    size_t total = 0;
    size_t card_count = 0;

    std::vector<Scratchcard *> cards;
    std::string filename = "4/input.txt";

    std::ifstream input_file(filename);
    std::string line;
    while(std::getline(input_file,line)){
        Scratchcard * card = new Scratchcard(line); 
        cards.push_back(card);
    }

    //Iterate through scratchcards
    size_t card_points = 0;

    //find winners
    for(Scratchcard * card: cards){
        //calculate points
        card_points = card->get_points();
        total += card_points;
    }
    //populate "copy" pointers
    for(Scratchcard * card : cards){
        for(size_t i = 0; i < card->winners_.size(); i++){
            card->card_copies_.push_back(cards[card->card_id_ + i]);
        }
    }
    //calculate total cards
    for(Scratchcard * card: cards){
        card_count += 1;
        card->recurse_cards(card_count);
    }

    std::cout << "The final result for Part 1 is: " << total << std::endl;
    std::cout << "The final result for Part 2 is: " << card_count << std::endl;

    return 0;
}