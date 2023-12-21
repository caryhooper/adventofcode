#include <string> 
#include <iostream> //
#include <algorithm> //reverse
#include <fstream> //ifstream
#include <map> //map
#include <vector>
#include <set>
#include <cmath>
#include <cassert>
#include <map>
#include <limits>
#include <numeric>
#include <deque>
#include "../helpers.hpp"

using namespace advent_of_code;

enum Voltage {lo,hi};

class Node{
    public:
        // Node(std::string identity, std::string snippet){
        //     id = identity;
        //     to_node_snippet = snippet;
        // }
        void send_signal(Voltage v, Node * from){
            std::cout << "send_signal() not implemented" << std::endl;
        }
        void send_to_dst(Voltage v){
            Signal s;
            for(Node * node : to_nodes){
                s.v = v;
                s.dest = node;
                signal_q.push_back(s);
            }
        }
        std::string to_node_snippet;
        std::string id;
        std::vector<Node *> to_nodes;
        std::vector<Node *> from_nodes;
};


typedef struct Signal{
    Voltage v;
    Node * dest;
} Signal;

std::deque<Signal> signal_q;

//std::vector<Node *> all_nodes;
size_t lo_count = 0;
size_t hi_count = 0;



class FlipflopNode : public Node {
    public:
        FlipflopNode(std::string identity, std::string snippet){
            id = identity;
            to_node_snippet = snippet;
        }
            //   % is a flip flop module (on/off, initially off).  low? --> flip.  off->on/high, on->off/low

        void send_signal(Voltage v,Node * from){
            if(v == lo){
                status = !status;

                if(status){ //if off to begin with and lo, send hi!
                    v = hi;
                }
            }
            send_to_dst(v);
        }
        bool status = false;
        
};

class ConjunctionNode : public Node {
    public:
        ConjunctionNode(std::string identity, std::string snippet){
            id = identity;
            to_node_snippet = snippet;
        }
        void init_memory(){
            Voltage v = lo;
            for(int i =0 ; i < (int) from_nodes.size(); i++){
                memory.push_back(v);
            }
        }
        bool all_hi(){
            for(Voltage v : memory){
                if(v == lo){
                    return false;
                }
            }
            return true;
        }
        void send_signal(Voltage v,Node * from){
            if(!memory_init){
                init_memory();
            }
     //       & is conjunction, remember most recent pulse from each connector, first remember low, 
    //      - update memory for low
    //      - remembers high for each input?  sends low.  o/w send high
            if(v == lo){
                for(int i = 0; i < (int) from_nodes.size(); i++){
                    if(from_nodes[i] == from){
                        memory[i] = lo;
                        break;
                    }
                }
                v = hi;
            } else {
                if(all_hi()){
                    v = lo;
                } 
            }
            send_to_dst(v);
        }

        bool memory_init = false;
        std::vector<Voltage> memory;
};
class BroadcastNode: public Node {
    public:
        BroadcastNode(std::string identity, std::string snippet){
            id = identity;
            to_node_snippet = snippet;
        }
        void push(Voltage v){
            send_to_dst(v);
        }
};
class SinkNode: public Node {
    public:
        SinkNode(std::string identity){
            id= identity;
        }
        void send_signal(Voltage v,Node * from){
        }
};

class NodeNetwork{
    public:
        void createNode(std::string input){
            // broadcaster -> sr, cg, dt, zs
            // %sk -> kx, qr
            // %xq -> dj
            // &vg -> rg
            std::string token;
            std::string id; 
            token = parse_until_delim(input);
            trim(token);
            if(token[0] == '%'){
                //create flipflop
                trim(token,'%');
                id = parse_until_delim(token);
                all_nodes.push_back(new FlipflopNode(id, input));
            } else if (token[0] == '&'){
                //create conjunction
                trim(token,'&');
                id = parse_until_delim(token);
                all_nodes.push_back(new ConjunctionNode(id, input));
            } else if(token.compare("broadcaster") == 0){
                //create broadcast node
                id = parse_until_delim(token);
                all_nodes.push_back(new BroadcastNode(id, input));
            } else {
                std::cout << "Sink module detected." << std::endl;
                all_nodes.push_back(new SinkNode(id));
            }
        }
        Node * get_node_by_id(std::string id){
            for(Node * this_node : all_nodes){
                if(this_node->id.compare(id) == 0  ){
                    return this_node;
                }
            }
            std::cout << "Warning... Node not found." << std::endl;
            return NULL;
        }
        void resolve_network(){
            std::string to_node;
            Node * this_node;
            //resolve "to"
            for(Node * node : all_nodes){
                parse_until_delim(node->to_node_snippet);
                while(!(node->to_node_snippet).empty()){
                    to_node = parse_until_delim(node->to_node_snippet,',');
                    trim(to_node);
                    this_node = get_node_by_id(to_node);
                    node->to_nodes.push_back(this_node);
                }
            }
            //resovle "from"
            for(Node * node : all_nodes){
                for(Node * to_node : node->to_nodes){
                    //add self as from node
                    to_node->from_nodes.push_back(node);
                }
            }
        }


        BroadcastNode * broadcast = NULL;
        std::vector<Node *> all_nodes;

};



int main(){
    //Pulse Propagation
    //   % is a flip flop module (on/off, initially off).  low? --> flip.  off->on/high, on->off/low
    //   & is conjunction, remember most recent pulse from each connector, first remember low, 
    //      - update memory for low
    //      - remembers high for each input?  sends low.  o/w send high
    //  broadcast. - send same to all dest 
    //size_t result = 0;
   

    std::string filename = "20/input.txt";
    std::ifstream input_file(filename);
    std::string line;
    
    NodeNetwork * nw = new NodeNetwork();
    while(std::getline(input_file,line)){
        //%dm -> tq, gp
        if(line.empty()){
            continue;
        }
        nw->createNode(line); 
    }
    nw->resolve_network();
    for(int i = 0; i < 1000; i++){
        nw->broadcast->push(lo);
        Signal s;
        while(!signal_q.empty()){
            s = signal_q.front();
            if(s.v == lo){
                lo_count += 1;
            } else {
                hi_count += 1;
            }
            signal_q.pop_front();
            s.dest->send_signal(s.v,s.dest);
        }
    }


    std::cout << "The final result for Part 1 is: " << hi_count*lo_count << std::endl;
    //std::cout << "The final result for Part 2 is: " << extrapolated_value_sum_rev << std::endl;

    return 0;
}