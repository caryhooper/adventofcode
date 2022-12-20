#include <bits/stdc++.h>    //std::stoi, size_t

enum RESULT  //https://stackoverflow.com/questions/35776576/how-to-have-a-tri-state-boolean-in-c
{
    NOT_CONCLUSIVE = -1,            // you can manually specify -1 to give it a special case value
    INCORRECT_ORDER,                // will be equal to 0
    CORRECT_ORDER                   // will be equal to 1
};

class PacketItem{
    public:
        std::vector<PacketItem *> list;
        std::string type;
        int64_t number;
        
        void printItem(){
            if(this->type.compare("int") == 0){
                std::cout << this->number << " ";
            } else {
                std::cout << "[";
                for(int i = 0 ; i < (int) this->list.size(); i++){
                    this->list[i]->printItem();
                    std::cout << ",";
                }
                std::cout << "]";
            }
        }

        void parseList(std::string& input){
            //[[1],[2,3,4]]
            int index = 0;
            std::string number;
            while(input[index] != ']'){
                //[1],[2,3,4]]
                    //1],[2,3,4]]
                //Item is the beginning of a list.
                if(input[index] == '['){
                    input.erase(0,1);
                    PacketItem packet_item = PacketItem();
                    packet_item.type = "list";
                    packet_item.parseList(input);
                    this->list.push_back(&packet_item);
                    continue;
                }

                //Item is a number within a list.
                number = std::to_string(input[index]);

                while(input[index+1] != ']' && input[index+1] != ','){
                    number.append(std::to_string(input[index]));
                    input.erase(0,1);
                }

                input.erase(0,1);
                int64_t value = std::stoi(number);
                PacketItem packet_item = PacketItem();
                packet_item.type = "int";
                packet_item.number = value;
                this->list.push_back(&packet_item);
            }
        }
        
        PacketItem(){}
        ~PacketItem(){};
};


//Convert long integer to list.
PacketItem * intToList(PacketItem * item){
    PacketItem * packet_list = new PacketItem();
    packet_list->type = "list";
    if(item->type.compare("int") == 0){
        packet_list->list.push_back(item);
    } else {
        std::cout << "Error type is not an int.  Returning empty list.\n";
    }
    return packet_list;
}

int compareItems(PacketItem * left, PacketItem * right){
    //Determine what kind of items these are.
    std::cout << "Comparing: ";
    left->printItem();
    std::cout << " and " ; 
    right->printItem();
    std::cout << "\n";

    if(left->type == "int" && right->type == "int"){
        //     If both values are integers, the lower integer should come first. If the left integer is lower than 
        //the right integer, the inputs are in the right order. If the left integer is higher than the right integer, 
        //the inputs are not in the right order. Otherwise, the inputs are the same integer; continue checking the 
        //next part of the input.
        if(left->number < right->number){
            return CORRECT_ORDER;
        }else if (left->number > right->number){
            return INCORRECT_ORDER;
        } else {
            return NOT_CONCLUSIVE;
        }
    }

    //Convert to PacketList if lone int.
    PacketItem * left_list;
    if(left->type == "int"){
        left_list = intToList(left);
    }else {
        left_list = left;
    }
    PacketItem * right_list;
    if(right->type == "int"){
        right_list = intToList(right);
    } else {
        right_list = right;
    }

    // If both values are lists, compare the first value of each list, then the second value, and so on. 
    //If the left list runs out of items first, the inputs are in the right order. If the right list runs out 
    //of items first, the inputs are not in the right order. If the lists are the same length and no comparison 
    //makes a decision about the order, continue checking the next part of the input.
    int size_left = (int) left_list->list.size();
    int size_right= (int) right_list->list.size();

    for(int i = 0; i < size_left; i++){
        if(i >= size_right){
            return INCORRECT_ORDER;
        }

        int result = compareItems(left_list->list[i], right_list->list[i]);
        if(result == CORRECT_ORDER){
            return CORRECT_ORDER;
        } else if (result == INCORRECT_ORDER) {
            return INCORRECT_ORDER;
        } else {
            continue;
        }
    }
    return CORRECT_ORDER;
}


int main(){
    //Init values / open file
    std::string filename = "13/input2.txt";
    std::ifstream file_stream(filename,std::ios::in);
    std::string line; 

    std::vector<std::pair<PacketItem,PacketItem>> packet_pairs;

    //Parse file
    while(file_stream){
        getline(file_stream,line);
        if(line.empty() || line[0] == '\n'){
            continue;
        }
        PacketItem p1 = PacketItem();
        p1.type = "list";
        p1.parseList(line);

        getline(file_stream,line);
        if(line.empty() || line[0] == '\n'){
            continue;
        }  
        PacketItem p2 = PacketItem();
        p2.type = "list";
        p2.parseList(line);

        packet_pairs.push_back(std::make_pair(p1,p2));
    }

    int64_t answer = 0;
    for(int i = 1; i <= (int) packet_pairs.size(); i++ ){
        int result = compareItems(&packet_pairs[i-1].first, &packet_pairs[i-1].second);
        if(result == CORRECT_ORDER){
            answer += result;
        }
    }

    std::cout << "The sum of the indices of the pairs in the correct order is " << answer << " (part1).\n";
    return 0;
}