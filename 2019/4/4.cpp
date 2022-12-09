#include <bits/stdc++.h>
#include <tgmath.h>

class PinRangeSieve {
    public:
        size_t hi;
        size_t lo;
        std::vector<size_t> range;

        void eliminatePasswords(){
            for(size_t i = 0; i < this->range.size();i++){
                //For each pin in the range...
                size_t num = this->range[i];

                //Separate into digits.
                std::deque<size_t> digits;
                size_t total = 0;
                size_t this_ten = 1;
                for(size_t j = 0; j < 6; j++){
                    this_ten *= 10;
                    size_t remainder = (num % this_ten) / (this_ten / 10);
                    total += remainder;
                    digits.push_front(remainder);
                }

                //Test for (1) two adjacent digits the same
                bool has_adjacent_digits = false;
                //Test for (2) digits never decreases

                bool is_ascending = true;
                for(size_t j = 0; j < 5; j++){
                    if(digits[j] == digits[j+1]){
                        if(j == 0){
                            //Part2 invalidate if there is a 3rd or 4th digit in row
                            if(digits[j+1] != digits[j+2]){
                                has_adjacent_digits = true;
                            }
                        } else if (j == 5){
                            if(digits[j] != digits[j-1]){
                                has_adjacent_digits = true;
                            }
                        } else {
                            if( (digits[j+1] != digits[j+2] ) && digits[j] != digits[j-1]){
                                has_adjacent_digits = true;
                            }   
                        }
                    }
                    if(digits[j] > digits[j+1]){
                        is_ascending = false;
                        break;
                    }
                }
                if(!has_adjacent_digits || !is_ascending){
                    this->range.erase(this->range.begin() + i);
                    i--;
                    continue;
                } //else {
                //     std::cout << "Possible pin found: " << num << ".\n";
                // }

            }
        }

        PinRangeSieve(size_t lo, size_t hi){
            this->lo = lo;
            this->hi = hi;
            for(size_t i = lo; i <= hi; i++){
                this->range.push_back(i);
            }
            this->eliminatePasswords();
        }
        ~PinRangeSieve(){}
};

int main(){
    std::string line;

    std::ifstream file_stream("4/input.txt",std::ios::in);

    while( file_stream ){
        std::getline(file_stream,line);
        if(line.empty()){
            continue;
        }
        size_t lo = 0;
        size_t hi = 0;
        auto iter = line.find('-');
        std::string lo_s = line.substr(0,iter);
        line.erase(0,iter + 1);
        lo = std::stoul(lo_s);
        hi = std::stoul(line);        

        PinRangeSieve prs = PinRangeSieve(lo,hi);
        //Part 1
        std::cout << "The solution is " << prs.range.size() << ".\n";
    }

}


