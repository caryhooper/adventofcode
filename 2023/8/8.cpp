#include <string> 
#include <iostream> //
#include <algorithm> //reverse
#include <fstream> //ifstream
#include <map> //map
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
            while(){

            }
        }

        std::map<std::string, DirectionalBigraphNode *> node_lookup_;

};

int main(){
    //Haunted Wasteland
    size_t steps  = 0;
    std::string filename = "8/input_test1.txt";

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


    return 0;
}