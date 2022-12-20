#include <bits/stdc++.h>    //std::stoi, size_t



union PacketItem {
    std::int64_t i;
    PacketList p;
};

class PacketList{
    private:

    public:
        std::vector<PacketItem> list;
        
        void parseList(std::string& input){
            //[[1],[2,3,4]]
            int index = 0;
            while(!input.empty()){
                char next_letter = input[index];
                //on [, create new list
                if(){

                }
                //on ], complete list
                //on number, add to current list
            }
        }
        
        PacketList(){}
        ~PacketList(){};
};

int main(){
    //Init values / open file
    std::string filename = "13/input2.txt";
    std::ifstream file_stream(filename,std::ios::in);
    std::string line; 

    //Parse file
    while(file_stream){
        getline(file_stream,line);
        if(line.empty() || line[0] == '\n'){
            continue;
        }
    }

    size_t answer = 0;
    std::cout << "The answer for this day is " << answer << " (part1).\n";
    return 0;
}