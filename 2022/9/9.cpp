#include <bits/stdc++.h>    //std::stoi, size_t

//Class "Point" to keep track of (x,y) coordinates of the rope path.
class Point{
    public:
        int x_val;
        int y_val;

        //For debug purposes only
        std::string pointToString(){
            std::string out = "(" + std::to_string(this->x_val) + "," + std::to_string(this->y_val) +  ")";
            return out;
        }

        Point(int x_val = 0, int y_val = 0){
            this->x_val = x_val; 
            this->y_val = y_val;
        }
        ~Point(){};
};

//"RopeKnot" class used to keep track of knot's movement history, unique locations visited, and current location.
//  provides methods to move the knot in various directions or follow the "head" (knot in front of it).
//  RopeKnot is unaware of any other knots before or behind it.
class RopeKnot{
    public:
        std::vector<Point> history;
        std::vector<Point> unique_history;
        Point current_location;

        //Adds Point to history and adds to unique_history only if it is not contained within unique_history
        void addHistory(Point p){
            //Make copy of point
            Point new_point = Point(p.x_val,p.y_val);
            this->history.push_back(new_point);
            
            //Determine if we should add it to unique_history.
            int match_count = 0;
            for(size_t i = 0; i < this->unique_history.size(); i++){
                if(this->unique_history[i].x_val == new_point.x_val && this->unique_history[i].y_val == new_point.y_val){
                    match_count++;
                }
            }
            if(match_count == 0){
                this->unique_history.push_back(new_point);
            }

        }

        //BEGIN MOVEMENT COMMANDS
        void moveUp(){
            this->current_location.y_val++;
            this->addHistory(this->current_location);
        }

        void moveLeft(){
            this->current_location.x_val--;
            this->addHistory(this->current_location);
        }

        void moveRight(){
            this->current_location.x_val++;
            this->addHistory(this->current_location);
        }

        void moveDown(){
            this->current_location.y_val--;
            this->addHistory(this->current_location);
        }

        void moveUpRight(){
            this->current_location.x_val++;
            this->current_location.y_val++;
            this->addHistory(this->current_location);
        }
        void moveUpLeft(){
            this->current_location.x_val--;
            this->current_location.y_val++;
            this->addHistory(this->current_location);
        }
        void moveDownRight(){
            this->current_location.x_val++;
            this->current_location.y_val--;
            this->addHistory(this->current_location);
        }
        void moveDownLeft(){
            this->current_location.x_val--;
            this->current_location.y_val--;
            this->addHistory(this->current_location);
        }
        //END MOVEMENT COMMANDS

        //Given the position of the next knot ahead of it, determine how this knot will move.
        void followHead(Point head_loc){
            //first determine if head is adjacent to tail
            int x_val_delta = abs(head_loc.x_val - this->current_location.x_val);
            int y_val_delta = abs(head_loc.y_val - this->current_location.y_val);
            //Case 1 - head and tail are adjacent
            if(y_val_delta <= 1 && x_val_delta <= 1){
                //Do nothing
                return;
            //Are the y_val values equal?
            } else if (y_val_delta == 0){
                //If so, determine if we need to move right or left
                if(this->current_location.x_val < head_loc.x_val){
                    moveRight();
                } else {
                    moveLeft();
                }
            //x values equal?
            } else if (x_val_delta == 0){
                //If so, determine if we need to move up or down
                if(this->current_location.y_val < head_loc.y_val){
                    moveUp();
                } else {
                    moveDown();
                }
            //Need to move diagonally_val
            } else if (
                this->current_location.x_val < head_loc.x_val &&
                this->current_location.y_val < head_loc.y_val 
            ){
                this->moveUpRight();
            }else if (
                this->current_location.x_val > head_loc.x_val &&
                this->current_location.y_val > head_loc.y_val 
            ){
                this->moveDownLeft();
            }else if (
                this->current_location.x_val < head_loc.x_val &&
                this->current_location.y_val > head_loc.y_val 
            ){
                this->moveDownRight();
            }else if (
                this->current_location.x_val > head_loc.x_val &&
                this->current_location.y_val < head_loc.y_val
            ){
                this->moveUpLeft();
            //Throw error here.  We should never reach this.
            } else {
                std::cout << "Error moving tail " << this->current_location.pointToString() << " to head " << head_loc.pointToString() << ".\n";
            }
        }

        RopeKnot(Point point = Point()){
            this->current_location = point;
            this->addHistory(this->current_location);
        }
        ~RopeKnot(){};
};

//Rope class manages all connected knots
class Rope{
    public:
        Point start_location;
        std::vector<RopeKnot> knots;

        //Added for Part2
        void addKnot(){
            this->knots.push_back(RopeKnot());
        }

        //called for each instruction given to Rope. For each movement, applies to head and all other knots.
        void readInstruction(std::string inst){
            char direction = inst[0];
            inst.erase(0,2);
            int magnitude = std::stoi(inst);

            //For each move, only directly move the first.
            for(int i = 0; i < magnitude; i++){
                if(direction == 'R'){
                    this->knots[0].moveRight();
                } else if (direction == 'L'){
                    this->knots[0].moveLeft();
                } else if (direction == 'U') {
                    this->knots[0].moveUp();
                } else if(direction == 'D') {
                    this->knots[0].moveDown();
                } else {
                    std::cout << "Error. Incorrect direction: " << direction << "\n";
                }
                //Resolve all other links in the chain.
                for(int n = 0; n < (int) this->knots.size() - 1; n++ ){
                    this->knots[n+1].followHead(this->knots[n].current_location);
                }
            }
        }

        Rope(){
            this->start_location = Point();
        }
        ~Rope(){};
};

int main(){
    //Init values / open file
    std::string filename = "9/input.txt";
    std::ifstream file_stream(filename,std::ios::in);
    std::string line; 

    //Initialize Rope object
    Rope rope = Rope(); 
    for(int i = 0; i < 10; i++){
        rope.addKnot();
    }

    //Parse file
    while(file_stream){
        getline(file_stream,line);
        if(line.empty() || line[0] == '\n'){
            continue;
        }
        //Pull in various directions
        rope.readInstruction(line);
    }

    //std::cout << "The number of unique places the tail has been is " << rope.tail.unique_history.size() << " (part1).\n";

    std::cout << "The number of unique places the tail has been is " << rope.knots[rope.knots.size() - 1].unique_history.size() << " (part2).\n";
    return 0;
}
