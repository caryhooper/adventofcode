#include <bits/stdc++.h>    //std::stoi, size_t

class CathodeRayTube{
    private:

    public:
        size_t cycles;
        ssize_t regX;
        size_t signal_count;
        std::vector<ssize_t> signals;

        //Pass in a string by reference to parse by a ' ' character.  Returns all characters up to the ' '
        //  and removes that token form the string.
        std::string getNextToken(std::string& line){
            auto iter = line.find(' ');
            std::string token = line.substr(0,iter);
            line.erase(0,iter + 1);
            return token;
        }

        void trackSignals(){
            //std::cout << "Note: signal tracked at " << this->cycles << ".\n";
            this->signal_count++;
            this->signals.push_back(this->cycles * this->regX);
        }

        void doInstruction(std::string opcode_s){
            std::string op = this->getNextToken(opcode_s);
            if(op.compare("noop") == 0){
                if( ((this->cycles + 20) % 40) == 0){
                    this->trackSignals();
                }
                this->cycles++;


            } else if (op.compare("addx") == 0) {
                std::string arg_s = this->getNextToken(opcode_s);
                ssize_t arg = std::stol(arg_s);

                //Check to see if signal is in the middle of an addx instruction
                if( ((this->cycles + 1 + 20) % 40) == 0){
                    this->cycles++;
                    this->trackSignals();
                    this->cycles++;
                } else if( ((this->cycles + 2 + 20) % 40) == 0){
                    this->cycles += 2;
                    this->trackSignals();
                } else {
                    this->cycles += 2;
                }
                this->regX += arg;
            } else {
                std::cout << "Warning! Unrecognized op: " << op << ".\n";
            }           
        }

        CathodeRayTube(){
            this->cycles = 0;
            this->regX = 1;
            this->signal_count = 0;
        }
        ~CathodeRayTube(){};
};

int main(){
    //Init values / open file
    std::string filename = "10/input.txt";
    std::ifstream file_stream(filename,std::ios::in);
    std::string line; 
    CathodeRayTube crt;

    //Parse file
    while(file_stream){
        getline(file_stream,line);
        if(line.empty() || line[0] == '\n'){
            continue;
        }
        crt.doInstruction(line);
    }

    ssize_t answer = 0;
    for(int i = 0; i < 6; i++){
        answer += crt.signals[i];
    }
    std::cout << "The sum of the signal strengths is " << answer << " (part1).\n";
    return 0;
}
