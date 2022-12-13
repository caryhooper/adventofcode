#include <bits/stdc++.h>    //std::stoi, size_t
#include <cmath>

class Monkey{

    public:
        std::deque<unsigned long long> items;
        // int number_of_items_held;
        std::function<int(int,int)> op;//https://stackoverflow.com/questions/10424119/storing-basic-arithmetic-operators-in-variables
        unsigned long long op_num1;
        unsigned long long op_num2;
        unsigned long long test;
        int to_if_true;
        int to_if_false;
        unsigned long long inspections;
        
        //Pass in a string by reference to parse by a ' ' character.  Returns all characters up to the ' '
        //  and removes that token form the string.
        std::string getNextToken(std::string& line){
            auto iter = line.find(' ');
            std::string token;
            if(iter == std::string::npos){
                token = line;
                line.erase(0,line.size());
                return token;
            }
            token = line.substr(0,iter);
            line.erase(0,iter + 1);
            return token;
        }

        std::string getLastToken(std::string line){
            std::string token;
            while(!line.empty()){
                token = this->getNextToken(line);
            }
            return token;
        }

        void parseOperation(std::string op_s){
            std::vector<std::string> tokens;
            while(!op_s.empty()){
                std::string token = this->getNextToken(op_s);
                tokens.push_back(token);
            }
            std::string operator_num1 = tokens[5];
            std::string our_op = tokens[6];
            std::string operator_num2 = tokens[7];
            if(our_op.compare("*") == 0){
                this->op = std::multiplies<int>();
            } else if (our_op.compare("+") == 0){
                this->op = std::plus<int>();
            }else {
                std::cout << "Error.  Cannot assign operator " << our_op << " to monkey.\n";
            }

            //Set op_num to 0 if we're to use "self"/old
            if(operator_num1.compare("old") == 0){
                this->op_num1 = 0;
            } else {
                this->op_num1 = std::stoi(operator_num1);
            }
            if(operator_num2.compare("old") == 0){
                this->op_num2 = 0;
            } else {
                this->op_num2 = std::stoi(operator_num2);
            }
        }
        
        void populateItems(std::string items_s){
            std::string token;
            int token_num = 0;
            while(!items_s.empty()){
                token = this->getNextToken(items_s);
                token_num++;
                if(token_num > 4){
                    token.erase(std::remove(token.begin(), token.end(), ','),token.end()); //removes all commas
                    this->items.push_back(std::stoi(token));
                }
            }
            // this->number_of_items_held = (int) this->items.size();
        }
        
        void addTest(std::string test_s){
            std::string token = this->getLastToken(test_s);
            this->test = std::stoi(token);
        }

        void addTrueResult(std::string true_s){
            std::string token = this->getLastToken(true_s);
            this->to_if_true = std::stoi(token);
        }

        void addFalseResult(std::string false_s){
            std::string token = this->getLastToken(false_s);
            this->to_if_false = std::stoi(token);
        }

        void doWorryOperation(unsigned long long& item){
            unsigned long long arg1 = 0;
            unsigned long long arg2 = 0;
            if(this->op_num1 == 0){
                arg1 = item;
            } else {
                arg1 = this->op_num1;
            }

            if(this->op_num2 == 0){
                arg2 = item;
            } else {
                arg2 = this->op_num2;
            }

            item = this->op(arg1,arg2);
        }

        void takeTurn(Monkey * to_true, Monkey * to_false){
            //Inspect and throw all items listed since beginning of turn.
            int items_at_beginning = (int) this->items.size();
            for(int i = 0; i < items_at_beginning; i++){
                unsigned long long item = this->items.front();     
                this->items.pop_front();   

                //Increase Worry
                this->doWorryOperation(item);

                //Decrease Worry
                //Commented for Part 1
                //item = std::floor(item / 3);

                //Throw to another monkey.
                if(item % (this->test) == 0){
                    to_true->items.push_back(item);
                } else {
                    to_false->items.push_back(item);
                }
                this->inspections++;
            }
        }

        Monkey(){
            this->inspections = 0;
        }
        ~Monkey(){};
};

class MonkeyBusiness{
    public:
        std::vector<Monkey *> monkeys;
        int rounds;
        uint64_t lcm;

    // void updateItemsHeld(){
    //     for(int i = 0; i < (int) this->monkeys.size(); i++){
    //         this->monkeys[i]->number_of_items_held = (int) this->monkeys[i]->items.size();
    //     }
    // }

    void findLCM(){
        std::vector<uint64_t> tests;
        uint64_t cieling = 1;
        for(int i = 0; i < (int) this->monkeys.size(); i++){
            cieling *= this->monkeys[i]->test;
            tests.push_back(this->monkeys[i]->test);
        }
        
        uint64_t lcm = 0;

        for(int i = 1; i <= (int) cieling; i++){
            for(int j = 0; j < (int) tests.size(); j++){
                if(i % tests[j] != 0){
                    j = (int) tests.size();
                } else {
                    if(j == (int) tests.size() - 1){
                        lcm = i; 
                        i = cieling;
                    }
                }
            }
        }
        this->lcm = lcm;
    }

    void reduceInts(){
        // this->findLCM();
        for(int i = 0; i < (int) this->monkeys.size(); i++){
            for(int j = 0; j < (int) this->monkeys[i]->items.size(); j++){
                if(this->monkeys[i]->items[j] > this->lcm){
                    uint64_t multiples = std::floor(this->monkeys[i]->items[j] / this->lcm);
                    this->monkeys[i]->items[j] = this->monkeys[i]->items[j] - (multiples * this->lcm);
                }
            }
        }
    }

    void doRound(){
        for(int i = 0; i < (int) this->monkeys.size(); i++){
            //Pass reference to recieving monkeys in takeTurn function.
            //std::cout << "Monkey " << i << " is taking a turn...\n";
            this->monkeys[i]->takeTurn(
                this->monkeys[this->monkeys[i]->to_if_true],
                this->monkeys[this->monkeys[i]->to_if_false]
            );
        }
        //this->updateItemsHeld();
        this->reduceInts();
        this->rounds++;
        // if(this->monkeys[2]->items.size() > 0 || this->monkeys[2]->items.size() > 0){
        //     std::cout << "Error.  We shouldn't be here.\n";
        // }
    }

    void listInspections(){
        for(int i = 0; i < (int) this->monkeys.size(); i++){
            std::cout << "Monkey " << i << " inspected items " << this->monkeys[i]->inspections << " times.\n";
        }
        std::cout << "\n";
    }

    void doRounds(int rounds){
        this->findLCM();
        for(int i = this->rounds; i <= rounds; i++){
            this->doRound();
            
            if( 
                i <= 21 ||
                i % 1000 == 0
            ){
                std::cout << "== After round " << i << " ==\n";
                this->listInspections();
            }
        }
    }

    unsigned long long getMonkeyBusiness(){
        std::vector<unsigned long long> interactions;
        for(int i = 0; i < (int) this->monkeys.size(); i++){
            interactions.push_back(this->monkeys[i]->inspections);
        }
        std::sort(interactions.begin(),interactions.end());
        std::cout << interactions.end()[-1]  << " and "  << interactions.end()[-2] << ".\n";
        return interactions.end()[-1]  * interactions.end()[-2];
    }

    MonkeyBusiness(){
        this->rounds = 1;
    }
    ~MonkeyBusiness(){}
};


int main(){
    //Init values / open file
    std::string filename = "11/input.txt";
    std::ifstream file_stream(filename,std::ios::in);
    std::string line; 
    MonkeyBusiness mb = MonkeyBusiness();

    //Parse file
    while(file_stream){
        getline(file_stream,line);
        if(line.empty() || line[0] == '\n'){
            continue;
        }
        //Start of new monkey
        std::cout << "Populating monkey...\n";
        if(line[0] == 'M'){
            Monkey * monkey = new Monkey();
            getline(file_stream,line);
            monkey->populateItems(line);
            getline(file_stream,line);
            monkey->parseOperation(line);
            getline(file_stream,line);
            monkey->addTest(line);
            getline(file_stream,line);
            monkey->addTrueResult(line);
            getline(file_stream,line);
            monkey->addFalseResult(line);
            mb.monkeys.push_back(monkey);
        }
    }
    //Part1
    mb.doRounds(20);
    unsigned long long monkey_business = mb.getMonkeyBusiness();
    std::cout << "The monkey_business after 20 rounds is " << monkey_business << " (part1).\n";

    //Part2
    mb.doRounds(10000);
    monkey_business = mb.getMonkeyBusiness();
    std::cout << "The monkey_business after 10000 rounds is " << monkey_business << " (part2).\n";

    return 0;
}
