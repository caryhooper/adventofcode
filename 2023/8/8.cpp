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
            for( std::pair<std::string,DirectionalBigraphNode *> map_val : node_lookup_){
                if(map_val.first.compare(lookup_id) == 0){
                    return map_val.second;
                }
            }
            std::cout << "Node with ID " << lookup_id << " not found!" << std::endl;
            return NULL;
        }

        void resolve_references(){
            for(std::pair<std::string,DirectionalBigraphNode *> map_val  : node_lookup_){
                map_val.second->left_ = get_node_from_id(map_val.second->left_id_);
                map_val.second->right_ = get_node_from_id(map_val.second->right_id_);
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

        bool is_in_vector(DirectionalBigraphNode * needle, std::vector<DirectionalBigraphNode *> haystack){
            for(DirectionalBigraphNode *  node : haystack){
                if(needle == node){
                    return true;
                }
            }
            return false;
        }

        size_t get_period(DirectionalBigraphNode * start, std::string directions){
            DirectionalBigraphNode * current_node = start;
            size_t steps = 0;;
            size_t directions_len = directions.size();
            size_t offset = steps % directions_len;

            do{
                char c = directions[offset];

                if(c == 'L'){//L
                    steps += 1;
                    current_node = current_node->left_;
                } else if (c == 'R'){ //R
                    steps += 1;
                    current_node = current_node->right_;
                } else {
                    std::cout << "Error reading character from directions: " << c << std::endl;
                }
                offset = steps % directions_len;

            }while(current_node != start || offset != 0);
            size_t period = steps / directions_len;
            return period;
        }

        void get_dists(DirectionalBigraphNode * start,std::string directions, std::vector<size_t>& dists){
            size_t steps = 0;
            DirectionalBigraphNode * current_node = start;
            std::vector<DirectionalBigraphNode *> ends;
            for(std::pair<std::string,DirectionalBigraphNode *> this_pair : node_lookup_){
                if(this_pair.first[2] == 'Z'){
                    ends.push_back(this_pair.second);
                }
            }
            size_t ends_len = ends.size();
            size_t directions_len = directions.size();
            while(ends_len != 0){
                char c = directions[steps % directions_len];
                if(is_in_vector(current_node, ends)){
                    ends_len -= 1;
                    dists.push_back(steps / directions_len);
                }

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
        }

        size_t navigate_graph_as_cyclic_ghost(std::string directions){
            //size_t period = 0; 
            //size_t distance_to_end = 0;
            std::vector<size_t> all_dists;
            
            for(std::pair<std::string,DirectionalBigraphNode *> this_pair : node_lookup_){
                if(this_pair.first[2] == 'A'){
                    //period = get_period(this_pair.second, directions);
                    get_dists(this_pair.second, directions, all_dists );
                }

            }
            size_t lcm = 1;
            for(size_t dist : all_dists){
                lcm = std::lcm(lcm, dist);
            }
            return lcm;
        }

        std::map<std::string, DirectionalBigraphNode *> node_lookup_;

};

int main(){
    //Haunted Wasteland
    size_t steps  = 0;
    size_t ghost_steps = 0;
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
    //ghost_steps = graph.navigate_graph_as_ghost(directions);
    ghost_steps = graph.navigate_graph_as_cyclic_ghost(directions);
    
    std::cout << "The final result for Part 2 is: " << ghost_steps << std::endl;



    return 0;
}