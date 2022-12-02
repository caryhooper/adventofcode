#include <cstdio>
#include <cstdlib>
#include <string>    
#include <ios>
#include <iostream>
#include <fstream>
#include <vector>
#include <bits/stdc++.h>

//Define strategy guide class that, once initialized, returns the score for two inputs.
//  This class also provides a way to store codecs (translations between letters and actions)
//  as well as methods to translate and return the throws needed for a certain outcome.
class StrategyGuide{
    private:
        //This map dictates what we should play depending on the opponent's play.  
        std::map<char,char> strategy_guide;
        std::map<char,std::string> opponent_codec;
        std::map<char,std::string> our_codec;
        std::map<char, std::string> outcome_codec;

        //This maps a play to the score
        std::map<std::string,uint64_t> score_guide;

    public:
        void init_strategy_map_item(char opponent_play,char our_play){
            this->strategy_guide.insert(std::make_pair(opponent_play,our_play));
        }
        void init_opponent_codec_item(char play, std::string play_string){
            this->opponent_codec.insert(std::make_pair(play,play_string));
        }
        void init_our_codec_item(char play, std::string play_string){
            this->our_codec.insert(std::make_pair(play,play_string));
        }
        void init_outcome_codec_item(char letter, std::string outcome_string){
            this->outcome_codec.insert(std::make_pair(letter,outcome_string));
        }
        //Uses opponent_codec to return rock, paper, or scissors.
        std::string getOpponentPlayByLetter(char letter){
            auto iter = this->opponent_codec.find(letter);
            if(iter == this->opponent_codec.end()){
                std::cout << "Error.  Could not find " << letter << " in opponent_codec.\n";
                return "";
            }
            return iter->second;
        }
        //Uses our_codec to return rock, paper, or scissors.
        std::string getOurPlayByLetter(char letter){
            auto iter = this->our_codec.find(letter);
            if(iter == this->our_codec.end()){
                std::cout << "Error.  Could not find " << letter << " in our_codec.\n";
                return "";
            }
            return iter->second;
        }
        //Uses outcome_codec to return rock, paper, or scissors.
        std::string getOutcomeByLetter(char letter){
            auto iter = this->outcome_codec.find(letter);
            if(iter == this->outcome_codec.end()){
                std::cout << "Error.  Could not find " << letter << " in outcome_codec.\n";
                return "";
            }
            return iter->second;
        }
        //Based on the opponent's RPS play and desired outcome (draw, win, loss), return the needed play.  //TODO - make into ENUM.
        std::string getThrowByOutcome(std::string opponent_play, std::string outcome){
            if(opponent_play.compare("rock") == 0){
                if(outcome.compare("draw") == 0){
                    return "rock";
                } else if (outcome.compare("loss") == 0){
                    return "scissors";
                } else {
                    return "paper";
                }
            } else if (opponent_play.compare("paper") == 0){
                if(outcome.compare("draw") == 0){
                    return "paper";
                } else if (outcome.compare("loss") == 0){
                    return "rock";
                } else {
                    return "scissors";
                }
            }else {
                //opponent plays scissors
                if(outcome.compare("draw") == 0){
                    return "scissors";
                } else if (outcome.compare("loss") == 0){
                    return "paper";
                } else {
                    return "rock";
                }
            }
            std::cout << "Error: outcome unknown for opponent play of " << opponent_play << " and outcome of " << outcome << "\n";
        }
        //Calculate the score of 1 round of RPS.
        uint64_t get_score(std::string opponent_play, std::string our_play){
            //Calculate score for our throw.
            if(opponent_play.empty() || our_play.empty()){
                //no play in line
                return 0;
            }

            auto iter = score_guide.find(our_play); //No error handling here.
            uint64_t throw_score = iter->second;

            uint64_t win_score;
            //Case for draw
            if(our_play.compare(opponent_play) == 0){
                win_score = 3;
            } else if (
            //Case for winning
                (opponent_play.compare("rock") == 0 && our_play.compare("paper") == 0) || //paper beats rock
                (opponent_play.compare("paper") == 0 && our_play.compare("scissors") == 0) || //scissors beats paper
                (opponent_play.compare("scissors") == 0 && our_play.compare("rock") == 0)  //rock beats scissors
            ){
                win_score = 6;
            } 
            else {
            //Case for loss
                win_score = 0;
            }
            return throw_score + win_score;
        }

        //Constructor
        StrategyGuide(){
            //Init throw score
            //X rock = 1        
            //Y paper = 2
            //Z scissors = 3
            this->score_guide.insert(std::make_pair("rock",1));
            this->score_guide.insert(std::make_pair("paper",2));
            this->score_guide.insert(std::make_pair("scissors",3));
        };
        //Destructor
        ~StrategyGuide(){};
};

int main(){
    //Opponent Play                  Our play
    //A-Rock, B-Paper, C-Scissors && X-Rock,Y-Paper,Z-Scissors

    //Initial Strategy A-Y, B-X, C-Z
    StrategyGuide sg = StrategyGuide();
    sg.init_strategy_map_item('A','Y');
    sg.init_strategy_map_item('B','X');
    sg.init_strategy_map_item('C','Z');

    sg.init_opponent_codec_item('A',"rock");
    sg.init_opponent_codec_item('B',"paper");
    sg.init_opponent_codec_item('C',"scissors");

    sg.init_our_codec_item('X',"rock");
    sg.init_our_codec_item('Y',"paper");
    sg.init_our_codec_item('Z',"scissors");

    sg.init_outcome_codec_item('X',"loss");
    sg.init_outcome_codec_item('Y',"draw");
    sg.init_outcome_codec_item('Z',"win");

   //Open file as an ifstream
    std::string filename = "2/input.txt";
    std::ifstream f(filename,std::ios::in);

    //Initialize variables to walk through file
    std::string line;
    uint64_t total_score = 0;

    uint64_t total_part2_score = 0;

    //Iterate through each line
    while(f){
        getline(f,line);
        line.erase(std::remove(line.begin(),line.end(), ' '), line.end());
        if(line.empty()){
            continue;
        }

        //Part 1
        total_score += sg.get_score(sg.getOpponentPlayByLetter(line[0]),sg.getOurPlayByLetter(line[1]));

        //Part2
        total_part2_score += sg.get_score(
            sg.getOpponentPlayByLetter(line[0]),
            sg.getThrowByOutcome(
                sg.getOpponentPlayByLetter(line[0]),
                sg.getOutcomeByLetter(line[1])
            )
        );
    }
    
    std::cout << "The total score with the initial strategy is: " << total_score << "\n";

    std::cout << "The total score with the Part 2 strategy is: " << total_part2_score << "\n";
}