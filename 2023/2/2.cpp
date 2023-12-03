#include <string> 
#include <iostream> //
#include <algorithm> //reverse
#include <fstream> //ifstream
#include <map> //map

class cubeRound{
    public:
        cubeRound(std::string line){
            //line includes up to  "3 green, 4 blue, 1 red"
            //assume at least one color pulled.
            size_t round_iter = 0;
            size_t color_iter = 0;
            std::string stub;
            int value = 0;
            std::string color; 
            do{
                round_iter = line.find(',');
                // if(iter == 0){
                //     iter = line.size();
                // }
                stub = line.substr(1,round_iter-1); //skip space, skip comma
                line.erase(0,round_iter+1); //erase until comma + comma
                color_iter = stub.find(' ');
                value = std::stoi(stub.substr(0,color_iter));
                color = stub.substr(color_iter+1,stub.size()-color_iter-1);
                if(color.compare("green") == 0){
                    green_ = value;
                } else if (color.compare("red") == 0){
                    red_ = value;
                } else if (color.compare("blue") == 0){
                    blue_ = value;
                } else {
                    std::cout << "Error parsing color: " << color << std::endl;                }
            }while(round_iter != std::string::npos); 
            
        }
        int green_ = 0;
        int red_ = 0;
        int blue_ = 0;
};

class cubeGame{
    public:
        cubeGame(std::string line){
            if(line.substr(0,5).compare("Game ") != 0){
                std::cout << "Error finding 'Game ' in line" <<std::endl;
                return;
            }
            line.erase(0,5);
            //parse line until ':'
            auto iter = line.find(':');
            if(iter == std::string::npos){
                std::cout << "Error finding ':' in line" <<std::endl;
                return;
            }
            game_id_ = std::stoi(line.substr(0,iter));
            line.erase(0,iter+1); //assume input is well-formed.

            //assume at least one round.
            do{
                iter = line.find(';');
                rounds_.push_back(cubeRound(line.substr(0,iter)));
                line.erase(0,iter+1); //erase leading space and semicolon
            }while(iter != std::string::npos);

            //Parse line for every ';' and then to end
        }
        int game_id_; 
        std::vector<cubeRound> rounds_;
};

class possibleCalculator{
    public:
        possibleCalculator(cubeGame * game){
            game_ = game;
        }
        bool checkPossible(){
            //for each red, green, blue find the maximum number in the set of rounds.
            int min_red = 0;
            int min_green = 0;
            int min_blue = 0;
            for(cubeRound round : game_->rounds_){
                if(round.blue_ > min_blue){
                    min_blue = round.blue_;
                }
                if(round.red_ > min_red){
                    min_red = round.red_;
                }
                if(round.green_ > min_green){
                    min_green = round.green_;
                }
            }
            if(min_red > this->red_ || min_blue > this->blue_ || min_green > this->green_){
                return false;
            } else {
                return true;
            }
        }
        cubeGame * game_;
        int green_ = 0;
        int red_ = 0;
        int blue_ = 0;
        //power is set of cubes equal to numbers of r*g*b
        int get_minimum_power(){
            //for each red, green, blue find the maximum number in the set of rounds.
            int min_red = 0;
            int min_green = 0;
            int min_blue = 0;
            for(cubeRound round : game_->rounds_){
                if(round.blue_ > min_blue){
                    min_blue = round.blue_;
                }
                if(round.red_ > min_red){
                    min_red = round.red_;
                }
                if(round.green_ > min_green){
                    min_green = round.green_;
                }
            }

            int min_power = min_green*min_red*min_blue;
            return min_power;
        }
};

//Cube Conundrum
int main(){
    //bag with cubes: r/g/b
    //Part 1 - track IDs which games possible with 12red, 13 green, 14blue

    //Goal - calculate sum of calibration values.
    std::ifstream input_file("2/input.txt");
    std::string line;
    int total = 0;
    int total_power = 0;
    possibleCalculator calc = possibleCalculator(NULL);
    calc.blue_ = 14;
    calc.green_ = 13; 
    calc.red_ = 12;
    while(std::getline(input_file,line)){
        if(line.size() < 13){
            break;
        }
        cubeGame * game = new cubeGame(line);
        calc.game_ = game;
        bool is_possible = calc.checkPossible();
        std::cout << "Game " << game->game_id_ << " is " << is_possible << std::endl;
        if(is_possible){
            total += game->game_id_;
        }
        total_power += calc.get_minimum_power();
        delete game;
    }
    std::cout << "The final result for Part 1 is: " << total << std::endl;
    std::cout << "The final result for Part 2 is: " << total_power << std::endl;

    return 0;
}