#include <bits/stdc++.h>

class IntcodeComputer {
    public:
        std::vector<size_t> instructions;
        size_t eip;

        void run_program(){
            while(1){
                //printf("first position is %u\n",machine_code[0]);
                unsigned int instruction = this->instructions[this->eip];
                unsigned int position1 = this->instructions[this->eip+1];    
                unsigned int position2 = this->instructions[this->eip+2];
                unsigned int result_position = this->instructions[this->eip+3];
                if (instruction == 99){
                    return;
                } else if (instruction == 1){
                    //addition
                    this->instructions[result_position] = this->instructions[position1] + this->instructions[position2];
                } else if (instruction == 2){
                    //multiplication
                    this->instructions[result_position] = this->instructions[position1] * this->instructions[position2];
                } else {
                    printf("Error. Current command is %u but expected 1, 2, or 99.",instruction);
                    exit(1);
                }
                this->eip+=4;
            }
        }

        void parseInstructionString(std::string instructions_s){
            while(instructions_s.length()  > 1){
                auto iter = instructions_s.find(',');
                std::string instruction_s = instructions_s.substr(0,iter);
                
                //Convert to size_t and push to end of instructions
                this->instructions.push_back(std::stoul(instruction_s));

                //Erase portion we just parsed.
                instructions_s.erase(0,iter + 1);
            }
        }

        IntcodeComputer(std::string instructions_s){
            this->eip = 0;
            this->parseInstructionString(instructions_s);
        }
        ~IntcodeComputer(){}
};


int main(){
    std::string line;
    size_t solution = 0;

    std::ifstream file_stream("2/input.txt",std::ios::in);

    while( file_stream ){
        std::getline(file_stream,line);
        if(line.empty()){
            continue;
        }
        IntcodeComputer computer(line);
        computer.run_program();
    }

    //Part 1
    std::cout << "The solution is " << solution << ".\n";
}


