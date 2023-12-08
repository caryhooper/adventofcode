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
#include "../helpers.hpp"

using namespace advent_of_code;

class DirectionalBigraphNode{
    public:

        DirectionalBigraphNode(std::string line){
            id_ = parse_until_delim(line);
            parse_until_delim(line,'(');
            left_id_ = parse_until_delim(line,',');
            right_id_ = parse_until_delim(line,')');
            trim(right_id_);
        }

        std::string id_;
        DirectionalBigraphNode * left_;
        DirectionalBigraphNode * right_;
        std::string left_id_;
        std::string right_id_;
} ;

class DirectionalBigraph{
    public:
        void addNode(std::string line){
            DirectionalBigraphNode * node = new DirectionalBigraphNode(line);
            node_lookup_.insert(std::pair<std::string, DirectionalBigraphNode *>(node->id_,node));
        }

        DirectionalBigraphNode * get_node_from_id(std::string lookup_id){
            for(auto const [id, node] : node_lookup_){
                if(id.compare(lookup_id) == 0){
                    return node;
                }
            }
            std::cout << "Node with ID " << lookup_id << " not found!" << std::endl;
            return NULL;
        }

        void resolve_references(){
            for(auto const [id, node]  : node_lookup_){
                node->left_ = get_node_from_id(node->left_id_);
                node->right_ = get_node_from_id(node->right_id_);
            }
        }

        size_t navigate_graph(std::string start_id, std::string end_id, std::string directions){
            size_t steps = 0;
            DirectionalBigraphNode * start = get_node_from_id(start_id);
            DirectionalBigraphNode * end   = get_node_from_id(end_id);
            DirectionalBigraphNode * current_node = start;
            size_t directions_len = directions.size();
            while(current_node != end){
                assert(current_node->left_ != NULL && current_node->right_ != NULL);

                char c = directions[steps % directions_len];
                
                if(c == 'L'){//L
                    steps += 1;
                    current_node = current_node->left_;
                } else if (c == 'R'){ //R
                    steps += 1;
                    current_node = current_node->right_;
                } else {
                    std::cout << "Error reading character from directions: " << c << std::endl;
                }
            }
            return steps;
        }

        bool all_nodes_at_end(std::vector<DirectionalBigraphNode *> current_nodes){
            for(DirectionalBigraphNode * node : current_nodes){
                if(node->id_[2] != 'Z'){
                    return false;
                }
            }
            return true;
        }

        unsigned long long navigate_graph_as_ghost(std::string directions){
            std::vector<DirectionalBigraphNode *> current_nodes;
            unsigned long long steps = 0;
            size_t directions_len = directions.size();
            for(auto const [id, node] : node_lookup_){
                if(id[2] == 'A'){
                    current_nodes.push_back(node);
                }
            }
            size_t nodes_len = current_nodes.size();
            std::cout << "Walking the graph with " << nodes_len << " nodes." << std::endl;

            while(!all_nodes_at_end(current_nodes)){
                char c = directions[steps % directions_len];
                assert(c == 'L' || c == 'R');
                steps += 1;
                if(c == 'L'){//L
                    for(int i = 0; i < (int) nodes_len; i++){
                        current_nodes[i] = current_nodes[i]->left_;
                    }
                } else { //R
                    for(int i = 0; i < (int) nodes_len; i++){
                        current_nodes[i] = current_nodes[i]->right_;
                    }
                }
            }
            return steps;
        }

        std::map<std::string, DirectionalBigraphNode *> node_lookup_;

};

int main(){
    //Haunted Wasteland
    size_t steps  = 0;
    unsigned long long ghost_steps = 0;
    std::string filename = "8/input.txt";

    std::ifstream input_file(filename);
    std::string line; 
    std::string directions;
    std::getline(input_file,directions);
    trim(directions);

    //graph setup
    DirectionalBigraph graph; 
    while(std::getline(input_file,line)){
        if(!line.empty()){
            graph.addNode(line);
        }
    }
    graph.resolve_references();


    //walk the graph.
    steps = graph.navigate_graph("AAA","ZZZ",directions);

    std::cout << "The final result for Part 1 is: " << steps << std::endl;
    ghost_steps = graph.navigate_graph_as_ghost(directions);
    std::cout << "The final result for Part 2 is: " << ghost_steps << std::endl;



    return 0;
}