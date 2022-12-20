#include <bits/stdc++.h>    //std::stoi, size_t

class Node{
    private:

    public:
        std::vector<Node *> next_paths;
        std::vector<Node *> possible_parents;
        Node * parent; 
        char letter;
        int x;
        int y;
        int score;
        int g_score; //g(n) // always 1
        int h_score; //h(n)

        int calcManhattanDistance(Node * node){
            return abs(node->x - this->x) + abs(node->y - this->y);
        }

        int calcDistanceProduct(Node * node){
            int x_delta = abs(node->x - this->x);
            int y_delta = abs(node->y - this->y);
            return x_delta * y_delta;
        }

        int distToEnd(Node * end){
            int hn = calcDistanceProduct(end);
            this->h_score = hn;
            return hn;
        }

        int updateScore(int parent_score){
            this->score = this->g_score + this->h_score + parent_score;
            return this->score;
        }

        Node(int x, int y, char mychar){
            this->letter = mychar;
            this->x = x;
            this->y = y;
            this->score = INT_MAX;
            this->g_score = 1;
            this->h_score = INT_MAX;

            this->parent = NULL;
        }
        ~Node(){};
};

class PathTraverser{
    private:

    public:
        std::deque<Node *> open;
        std::deque<Node *> close;

        std::vector<Node *> path;

        Node * start;
        Node * end;

        std::vector<std::vector<Node *>> input_map;
        
        //this->input_map[i][j]
        int cols; // corresponds to j ~ x
        int rows; // corresponds to i  ~ y

        // int calcManhattanDistance(Node * node){
        //     return abs(node->x - this->end->x) + abs(node->y - this->end->y);
        // }

        int returnLowestScore(std::deque<Node *>& our_deque){
            int lowest_score = INT_MAX;
            for(int i = 0; i < (int) our_deque.size(); i++){
                if(our_deque[i]->score < lowest_score){
                    lowest_score = our_deque[i]->score;
                }
            }
            return lowest_score;
        }

        Node * popFirstNodeByScore(int score){
            Node * node = NULL;
            int i = 0;
            for(; i < (int) this->open.size(); i++){
                node = this->open[i];
                if(node->score == score){
                    break;
                }
            }
            if(node != NULL){
                this->open.erase(this->open.begin() + i);
            }
            return node;
        }

        Node * getNodeByXY(std::deque<Node *> our_deque, int x, int y){
            for(int i = 0; i < (int) our_deque.size(); i++){
                if(our_deque[i]->x == x && our_deque[i]->y == y){
                    return our_deque[i];
                }
            }
            return NULL;
        }

        void updateNodeInDeque(std::deque<Node *>& our_deque, Node * our_node){

        }

        void walkGraph(){
            this->open.push_back(this->start);

        }

        void doAStar(Node * start_node){
            //https://mat.uab.cat/~alseda/MasterOpt/AStar-Algorithm.pdf
            this->open.push_back(start_node);

            //Keep track of where each node came from
            //keep track of lowest cost for each node.

            Node * current;

            while(!this->open.empty()){
                //f(n) = g(n) + h(n)
                //n is the next node on the path
                //g(n) is the cost of the path from start node to n and
                //h(n) is an estimator for the cost of the cheapest path from n to goal.  Never overestimate. Manhattan distance is our metric (shortest number of steps)

                int lowest_score = this->returnLowestScore(this->open);
                current = this->popFirstNodeByScore(lowest_score);

                if(current == this->end){
                    return;
                }
                
                std::vector<Node *> successors = current->next_paths;

                //Set parent for downstream nodes.
                for(int i = 0; i < (int) successors.size(); i++){
                    Node * successor = successors[i];
                    successor->parent = current;
                
                    //check if node with same position is in OPEN list with lower f... skip .. otherwise add to open list
                    //check if node with same position is in CLOSED list ... skip


                    // int temp_g_score = current->g_score + 1;
                    int temp_score = successor->g_score + successor->h_score + current->score;

                    //Node * open_node = this->getNodeByXY(this->open, successor->x, successor->y);
                    
                    if(temp_score < successor->score && current != successor->parent){ //ensure we're not setting the current's parent to its own child
                        successor->score = temp_score;
                        successor->parent = current;
                    }
                    //When wouldn't we add the successor to teh open() list?  If it's already in this->open with a lower f value
                    Node * open_node  = this->getNodeByXY(this->open,successor->x,successor->y);
                    Node * close_node = this->getNodeByXY(this->close,successor->x, successor->y);

                    if( (open_node == NULL || open_node->score > successor->score) && close_node == NULL ){
                        this->open.push_back(successor);
                    }
                }
                this->close.push_back(current);
            }
        }

        std::deque<Node *> tracePath(Node * ending_node){
            std::deque<Node *> path;
            do{
                path.push_front(ending_node);
                ending_node = ending_node->parent;
            } while(ending_node != NULL);
            return path;
        }

        std::set<std::pair<int,int>> find_neighbors(int x, int y){
            //Function adapted from AoC 2021 Day 12 (Python).
            //Find all neighbors (set of tuples) adjacent to a x,y coord.
            //Note... no diagonals. 
            std::set<std::pair<int,int>> neighbors;
            std::vector<std::pair<int,int>> pairs = {
                {0,1},
                {0,-1},
                {1,0},
                {-1,0}
            };
            for(int i = 0; i < (int) pairs.size(); i++){
                std::pair pair = pairs[i];
                if( (x + pair.first >= this->rows) || (x + pair.first < 0)){
                    pair.first = 0;
                }
                if( (y + pair.second >= this->cols) || (y + pair.second < 0)){
                    pair.second = 0;
                }
                neighbors.insert(std::pair(pair.first + x, pair.second + y));
            }
            auto iter = neighbors.find({x,y});
            if(iter != neighbors.end()){
                neighbors.erase(iter);
            }
            return neighbors;
        }

        void drawGraphEdges(){
            //i == y, j == x
            for(int i = 0; i < this->rows; i++){
                for(int j = 0; j < this->cols; j++){
                    Node * this_node = this->input_map[i][j];
                    std::set<std::pair<int,int>> neighbors = this->find_neighbors(i,j);
                    for(auto iter :  neighbors){
                        Node * neighbor = this->input_map[iter.first][iter.second];
                        if( neighbor->letter - this_node->letter == 1 || neighbor->letter - this_node->letter == 0){
                            this_node->next_paths.push_back(neighbor);
                            neighbor->possible_parents.push_back(this_node);
                        }
                    }
                    this_node->distToEnd(this->end);
                }
            }
        }

        void parseLine(std::string line){
            std::vector<Node *> row;
            for(int i = 0; i < (int) line.length(); i++){
                char this_char = line[i];
                Node * this_node = new Node(i,this->rows,this_char);
                row.push_back(this_node);

                if(this_char == 'S'){
                    this->start = this_node;
                    this_node->letter = 'a' - 1;
                } 
                if(this_char == 'E'){
                    this->end = this_node;
                    this_node->letter = 'z' + 1;
                } 
            }
            this->cols = row.size() - 1;
            this->rows++;
            this->input_map.push_back(row);
        }

        PathTraverser(){
            this->cols = 0;
            this->rows = 0;
        }
        ~PathTraverser(){};
};

int main(){
    //Init values / open file
    std::string filename = "12/input2.txt";
    std::ifstream file_stream(filename,std::ios::in);
    std::string line; 

    PathTraverser pt = PathTraverser();

    //Parse file
    while(file_stream){
        getline(file_stream,line);
        if(line.empty() || line[0] == '\n'){
            continue;
        }
        pt.parseLine(line);
    }

    pt.drawGraphEdges();
    pt.doAStar(pt.start);

    std::deque<Node *> shortest_path = pt.tracePath(pt.end);

    size_t answer = shortest_path.size();
    std::cout << "The answer for this day is " << answer << " (part1).\n";
    return 0;
}
