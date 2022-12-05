// #include <cstdio>
// #include <cstdlib>
// #include <string>
// #include <iostream>
#include <bits/stdc++.h>    //std::stoi

//Container class exists to parse the "[" and "]" from the container and contain the letter.
class Container{
    private:
        char parse_container(std::string container){
            container.erase(std::remove(container.begin(),container.end(), ' '), container.end());
            container.erase(std::remove(container.begin(),container.end(), '['), container.end());
            container.erase(std::remove(container.begin(),container.end(), ']'), container.end());
            return container[0];
        }
    public:
        char letter;

        void print_container(){
            std::cout << "[" << this->letter << "] ";
        }

        Container(std::string input_string){
            this->letter = parse_container(input_string);
        };
        ~Container(){};
};

//Movement command object parses and tracks the command string (english version).
class MovementCommand{
    private:
        std::vector<std::string> tokenizeString(std::string our_string, char delim){
            std::vector<std::string> tokens;
            while( our_string.length() > 1 ){
                auto iter = our_string.find(delim);
                std::string token = our_string.substr(0,iter);
                tokens.push_back(token);
                our_string.erase(0,iter+1);
            }
            tokens.push_back(our_string);
            return tokens;
        }

        void parseMovementCommand(){
            std::vector<std::string> tokens = this->tokenizeString(this->command,' ');
            this->qty = std::stoi(tokens[1]);
            this->from_stack = std::stoi(tokens[3]); 
            this->to_stack = std::stoi(tokens[5]);
        }


    public:
        int qty;
        int from_stack;
        int to_stack;
        std::string command;
        
        MovementCommand(std::string command){
            this->command = command;
            this->parseMovementCommand();
        };
        ~MovementCommand(){};
};

//Class Container Yard is the main class for this program.  It draws the container yard for visualizing.
//  In addition, it parses the initial state and keps track of the order of movement commands.
//  Primarily, it tracks all containers' order within "yard" and is able to process movement commands
//  either using the crane version 9000 or crane version 9001.  Once all input is parsed, the 
//  "doMovements()" method drains the movement list and changes the yard's state.
class ContainerYard{
    public:
        std::deque<MovementCommand> movement_commands = std::deque<MovementCommand>();
        std::vector<std::deque<Container>> yard;
        
        //Translates the assigned number to actual index within the yard vector
        int getIndexFromNumber(int num){
            return num - 1;
        }
        
        int getNumberFromIndex(int index){
            return index + 1;
        }
        
        //Parses the English movement string into a MovementCommand object.  Stores locally
        void parseMovementString(std::string movement){
            MovementCommand mc = MovementCommand(movement);
            this->movement_commands.push_back(mc);
        }

        //Parses initial state string into a Container object then stores in yard.
        void parseStateString(std::string state){
            //Parse line 3 characters at a time, incrementing whenever we see a space.
            int current_index = 0;
            std::string container_string;

            for(size_t i = 0; i < state.length(); i = i + 3){

                size_t iteration_num = (i - current_index) / 3;
                if( iteration_num >= this->yard.size()){
                    std::deque<Container> stack = std::deque<Container>();
                    this->yard.push_back(stack);
                }

                //Only allocate new stack if we've found additional stacks.
                //NEED BETTER PARSING LOGIC HERE!!!!
                

                //4,8,12,16
                if(state[i] == ' '){
                    current_index++;
                    //Funky math here to skip the space delimiter.
                    i++;
                } 
                container_string = state.substr(i,3);
                if(container_string.compare("   ") == 0){
                    //skip if blank container
                    //incremented current_index already
                    continue;
                }
                if(container_string.compare("1  ") == 0){
                    //end if we reach the last line.
                    return;
                }
                Container ctr = Container(container_string);
                std::cout << "Adding letter " << ctr.letter << " to stack " << this->getNumberFromIndex(current_index) << ".\n";
                this->yard[current_index].push_front(ctr); //push to the front here so we can pop from the back later
            }
        }

        //Function to print the state of the yard for debugging and bc its cool.
        void visualizeYard(){
            size_t max_stack_length = 0;
            size_t number_of_stacks = this->yard.size();
            for(size_t i = 0; i < number_of_stacks; i++){
                if(max_stack_length < this->yard[i].size()){
                    max_stack_length = this->yard[i].size();
                }
            }

            //Loop through every stack 1 to n
            for(size_t y_val = 0; y_val < max_stack_length; y_val++){
                //Iterate through all stacks in yard
                for(size_t i = 0; i < number_of_stacks; i++){
                    //Only print if its length is greater than or equal to the current y val
                    std::deque<Container> stack = this->yard[i];
                    if(stack.size() >= (max_stack_length - y_val)){
                        // auto iter = stack.end() - y_val;
                        std::cout << "[" << stack.at(max_stack_length - y_val - 1).letter << "] ";
                    } else {
                        std::cout << "    ";
                    }
                }
                std::cout << "\n" ;
            }
            for(size_t i=1;i <= number_of_stacks; i++){
                std::cout << " " << i << "  ";
            }
            std::cout << "\n";
            for(size_t i = 0; i < (number_of_stacks*4); i++){
                std::cout << "-";
            }
            std::cout << "\n" ;
        }

        //Moves containers in LIFO format to the new row (crane version 9000 aka part 1)
        void doNextMovement(MovementCommand mc){
            for(int i = 0; i < mc.qty; i++){
                Container ctr = this->yard[this->getIndexFromNumber(mc.from_stack)].back();
                this->yard[this->getIndexFromNumber(mc.from_stack)].pop_back();
                this->yard[this->getIndexFromNumber(mc.to_stack)].push_back(ctr);
            }
        }

        //Moves containers in FIFO format to the new row (crane version 9001 aka part 2) 
        void doNextMovement9001(MovementCommand mc){
            //Instead of popping/pushing one at a time, we load into a buffer stack (LIFO).
            std::deque<Container> buffer;
        
            //load to back of buffer
            for(int i = 0; i < mc.qty; i++){
                Container ctr = this->yard[this->getIndexFromNumber(mc.from_stack)].back();
                this->yard[this->getIndexFromNumber(mc.from_stack)].pop_back();
                buffer.push_back(ctr);
            }

            //pop from back of buffer
            for(int i = 0; i < mc.qty; i++){
                Container ctr = buffer.back();
                this->yard[this->getIndexFromNumber(mc.to_stack)].push_back(ctr);
                buffer.pop_back();
            }

        }

        //Process all stored movements and solve the puzzle.
        void doMovements(int crane_version = 9000){
            this->visualizeYard();

            while(this->movement_commands.size() > 0){
                MovementCommand mc = this->movement_commands.front();
                std::cout << "DOING: " << mc.command << ".\n";
                this->movement_commands.pop_front();
                //std::cout << "Size of movement_commands is " << this->movement_commands.size() << "\n";
                if(crane_version == 9000){
                    this->doNextMovement(mc);
                } else {
                    this->doNextMovement9001(mc);
                }
                this->visualizeYard();
            }
        }

        //Retrieves the answer from the final state of the ContainerYard.
        std::string getMessage(){
            std::string message;
            for(size_t i = 0; i < yard.size(); i++){
                message.push_back(this->yard[i].back().letter);
            }
            return message;
        }
        
        ContainerYard(){};
        ~ContainerYard(){};
};

//Initial State
// [P]     [L]         [T]            
// [L]     [M] [G]     [G]     [S]    
// [M]     [Q] [W]     [H] [R] [G]    
// [N]     [F] [M]     [D] [V] [R] [N]
// [W]     [G] [Q] [P] [J] [F] [M] [C]
// [V] [H] [B] [F] [H] [M] [B] [H] [B]
// [B] [Q] [D] [T] [T] [B] [N] [L] [D]
// [H] [M] [N] [Z] [M] [C] [M] [P] [P]
//  1   2   3   4   5   6   7   8   9 

//First movement command
// move 8 from 3 to 2

int main(){
    //Init values / open file
    std::string filename = "5/input.txt";
    std::ifstream file_stream(filename,std::ios::in);
    std::string line; 
    int empty_line_counter = 0;

    //Track parsed state as a vector of deques within ContainerYard class.
    ContainerYard cy = ContainerYard();

    //Parse file
    while(file_stream){
        getline(file_stream,line);
        if(line.empty() || line[0] == '\n'){
            empty_line_counter++;
            continue;
        }

        //Parse current state
        if(empty_line_counter == 0){
            cy.parseStateString(line);
        } else {
            //parse movement instructions
            cy.parseMovementString(line);
        }
    }

    //Part1
    //cy.doMovements();
    //Part2
    cy.doMovements(9001);
    std::string message = cy.getMessage();

    std::cout << "The message on the top of each stack is " << message << " (part2).\n";
    return 0;
}

//launch.json
// {"configurations": [{
//     "name": "(gdb) Launch",
//     "type": "cppdbg",
//     "request": "launch",
//     "program": "${workspaceFolder}/${fileBasenameNoExtension}.elf",
//     "stopAtEntry": false,
//     "cwd" : "${workspaceFolder}",
//     "externalConsole":false,
//     "MIMode": "gdb",
//     "miDebuggerPath": "/usr/bin/gdb",
//     "setupCommands":[{
//         "description":"Enable pretty-printing for gdb",
//         "text":"--enable-pretty-printing",
//         "ignoreFailures" : true
//     }],
//     "preLaunchTask": "build"
// }]}
  
//tasks.json
// {
//     "version": "2.0.0",
//     "tasks": [
//         {
//             "label": "build",
//             "type": "shell",
//             "command": "/usr/bin/g++",
//             "group": "build",
//             "args": [
//                 "-g3",
//                 "-Wall",
//                 "-Werror",
//                 "-fno-omit-frame-pointer",
//                 "-Wno-format-security",
//                 "${fileBasenameNoExtension}/${fileBasename}",
//                 "-o",
//                 "${workspaceFolder}/${fileBasenameNoExtension}.elf",
//                 "&&",
//                 "chmod",
//                 "+x",
//                 "${workspaceFolder}/${fileBasenameNoExtension}.elf"
//             ]
//         },
//         {
//             "type": "cppbuild",
//             "label": "C/C++: g++ build active file",
//             "command": "/usr/bin/g++",
//             "args": [
//                 "-fdiagnostics-color=always",
//                 "-g",
//                 "${file}",
//                 "-o",
//                 "${fileDirname}/${fileBasenameNoExtension}"
//             ],
//             "options": {
//                 "cwd": "${fileDirname}"
//             },
//             "problemMatcher": [
//                 "$gcc"
//             ],
//             "group": {
//                 "kind": "build",
//                 "isDefault": true
//             },
//             "detail": "Task generated by Debugger."
//         }
//     ]
// }