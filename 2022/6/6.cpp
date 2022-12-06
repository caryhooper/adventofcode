#include <bits/stdc++.h>    //std::stoi, size_t, etc.

//SignalAnalyzer object recieves a signal and the window size (number of characters to analyze at a time)
//  It iterates through each character, tracked by "index" and observes each Start-Of-* markers, depending
//  on the message or packet size.
class SignalAnalyzer{
    public:
        std::string signal;         //Entire message
        size_t index;               //Tracks the current index
        std::string window;         //Rolling window of fixed size to compare signals/letters within
        std::vector<int> markers;   //Tracks all markers found so far.  Not needed for AoC

        //Uses a "set" to return the number of unique characters in the provided string
        size_t uniqueElements(std::string query_string){
            std::set<char> letter_set;
            //Add all chars to set
            for(size_t i = 0; i < query_string.length(); i++){
                letter_set.insert(query_string[i]);
            }
            //Return length of set
            return letter_set.size();
        }

        //Returns the index of the next marker, or 0 if end of signal is reached
        size_t findNextMarker(){
            //Continue until a unique string is encountered
            while(this->window.length() != uniqueElements(this->window)){
                //Remove the first letter
                this->window.erase(0,1);
                //Before we access this->index of this->signal, apply bounds checking and return 0 if we've made it to the end.
                if(this->index >= this->signal.length()){
                    return 0;
                }
                //Append char to string
                this->window.push_back(this->signal[index]);
                this->index++;
            }
            markers.push_back(index);
            return index;
        }

        SignalAnalyzer(std::string input_string,int window_size){
            this->signal = input_string;
            //Initialize this->window with the first window_size characters
            this->window = input_string.substr(0,window_size);
            this->index = window_size;
        };
        ~SignalAnalyzer(){};
};

//Start of packet - four characters all different.
//Identify index of last char of start of packet +  1

int main(){
    //Init values / open file
    std::string filename = "6/input.txt";
    std::ifstream file_stream(filename,std::ios::in);
    std::string line; 
    size_t first_sop_index;
    size_t first_som_index;


    //Parse file
    while(file_stream){
        getline(file_stream,line);
        if(line.empty() || line[0] == '\n'){
            continue;
        }
        //Part 1
        SignalAnalyzer signal_analyzer = SignalAnalyzer(line,4);
        first_sop_index = signal_analyzer.findNextMarker();
        //Part 2
        SignalAnalyzer signal_analyzerP2 = SignalAnalyzer(line,14);
        first_som_index = signal_analyzerP2.findNextMarker();
    }

    std::cout << "The number of characters that need to be processed to find the first SOP marker is " << first_sop_index << " (part1).\n";
    std::cout << "The number of characters that need to be processed to find the first SOM marker is " << first_som_index << " (part1).\n";

    return 0;
}
