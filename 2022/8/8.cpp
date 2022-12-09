#include <bits/stdc++.h>    //std::stoi, size_t

//Struct to keep track of the scenic score components for each Tree (part2)
typedef struct {
    size_t up;
    size_t down;
    size_t left;
    size_t right;
} scenic_score_t;

//Tree class keeps track of location, height, and scenic value of each tree.
class Tree{
    private:
    public:
        size_t height;
        size_t x;
        size_t y;
        bool is_visible;
        size_t scenic_score;
        scenic_score_t score;
        
        //Avoid calculating scenic score unless tree's scenic scores are populated by Forest class. 
        void calculate_scenic_score(){
            this->scenic_score = this->score.up * this->score.down * this->score.left * this->score.right;
       }

        //Used for debugging/pretty printing
        std::string getCoords(){
            return "(" + std::to_string(this->x) + "," + std::to_string(this->y) + ")";
        }
        
        Tree(size_t x, size_t y, size_t height){
            this->height = height;
            this->x = x;
            this->y = y;
            this->is_visible = false;
            this->scenic_score = 0;
            this->score.up = this->score.down = this->score.left = this->score.right = 0;
        };
        ~Tree(){};
};

//Forest keeps track of a grid of trees and counts trees visible from the exterior (part1)
//  In addition, it calculates the scenic score of each tree.
class Forest{
    private:

    public:
        size_t visible_trees;
        // this->grid[y][x] //Assumes rectangular grid.
        std::vector<std::vector<Tree>> grid;
        size_t rows;
        size_t cols;
        Tree * most_scenic;

        //Helper function for Part1 that returns true if all tree heights in a vector are less than the given Tree's height.
        bool allNumbersLessThanValue(std::vector<Tree> numbers, Tree * tree){
            for(size_t i = 0; i < numbers.size(); i++){
                if(numbers[i].height >= tree->height){
                    return false;
                }
            }
            return true;
        }

        //Given a vector of Trees, calculate the scenic score in a single direction. 
        //  parameter reverse is indicated if we need to count down or left.
        size_t countScenery(std::vector<Tree> numbers, Tree * tree, bool reverse = false){
            size_t scenery_count = 0;

            for(int i = 0; i < (int) numbers.size(); i++){
                int index;
                if(reverse){
                    index = (int) numbers.size() - 1 - i;
                } else {
                    index = i;
                }

                if(tree->height > numbers[index].height){
                    scenery_count++;
                }
                if (tree->height <= numbers[index].height){
                    scenery_count++;
                    return scenery_count;
                }
            }
            return scenery_count;
        }


        void findVisibleTreesHorizontally(size_t x, size_t y){
            //If tree is on the edge, its scenic score is 0 and it it automatically visible.
            if(x == 0 || x == (this->cols - 1) || y == 0 || y == (this->rows - 1)){
                this->grid[y][x].is_visible = true;
                this->grid[y][x].scenic_score = 0;
                return;
            }

            //Initialize vectors of trees on each side of the given tree.
            std::vector<Tree> row = this->grid[y];
            std::vector<Tree> left_side = std::vector<Tree>(row.begin(),row.begin() + x);
            std::vector<Tree> right_side = std::vector<Tree>(row.begin() + x + 1, row.end());
            //Pass this tree by reference so we can set the scenic score with countScenery()
            Tree * this_tree = &this->grid[y][x];

            //Determine if tree is visible.
            if(
                this->allNumbersLessThanValue(left_side,this_tree) ||
                this->allNumbersLessThanValue(right_side,this_tree)
            ){
                this->grid[y][x].is_visible = true;
            }

            //Part2
            //Calculate right and left scenic scores.
            this_tree->score.left = this->countScenery(left_side,this_tree,true);
            this_tree->score.right = this->countScenery(right_side,this_tree);
            
        }

        void findVisibleTreesVertically(size_t x, size_t y){  
            //Determine if tree is on the edge.  If so, scenic score is 0 and it is automatically visible.
            if(x == 0 || x == (this->cols - 1) || y == 0 || y == (this->rows - 1)){
                this->grid[y][x].is_visible = true;
                this->grid[y][x].scenic_score = 0;
                return;
            }

            //constant X value
            std::vector<Tree> col;
            //Populate column vector
            for(size_t i = 0; i < this->rows; i++){
                col.push_back(this->grid[i][x]);
            }

            //Split into trees above and trees below col
            std::vector<Tree> top = std::vector<Tree>(col.begin(),col.begin() + y);
            std::vector<Tree> bottom = std::vector<Tree>(col.begin() + y + 1, col.end());
            Tree * this_tree = &this->grid[y][x];

            //Part 1 - determine if tree is visible.
            if(
                this->allNumbersLessThanValue(top,this_tree) ||
                this->allNumbersLessThanValue(bottom,this_tree)
            ){
                this->grid[y][x].is_visible = true;
            }

            //Part2 - calculate scenic score in top & bottom.
            this_tree->score.up = this->countScenery(top,this_tree,true);
            this_tree->score.down = this->countScenery(bottom,this_tree);
        }

        void findVisibleTrees(){
            //For each tree in the grid...
            for(size_t y = 0; y < this->grid.size(); y++){
                for(size_t x = 0; x < this->grid[y].size(); x++){
                    //find visible trees both horizontally and vertically 
                    this->findVisibleTreesHorizontally(x,y);
                    this->findVisibleTreesVertically(x,y);
                }
            }
        }

        //Called after findVisibleTrees function to iterate through forest and count visible trees and find the most scenic tree.
        size_t calculateVisibleTrees(){
            this->visible_trees = 0;
            this->most_scenic = &this->grid[0][0];

            for(size_t y = 0; y < this->grid.size(); y++){
                for(size_t x = 0; x < this->grid[y].size(); x++){
                    // Tree tree =  this->grid[y][x];
                    //Part2
                    this->grid[y][x].calculate_scenic_score();
                    if(this->grid[y][x].scenic_score > this->most_scenic->scenic_score){
                        this->most_scenic = &this->grid[y][x];
                    }

                    //Part1
                    if(this->grid[y][x].is_visible){
                        this->visible_trees++;
                    }
                }
            }
            return this->visible_trees;
        }

        //Helper function to parse in trees and their heights one at a time.
        void parseLineOfTrees(std::string tree_input){
            std::vector<Tree> row;
            for(size_t n = 0; n < tree_input.length(); n++){
                row.push_back(
                    Tree(
                        n,
                        this->rows,
                        tree_input[n] - '0'
                    )
                );
            }
            this->grid.push_back(row);
            this->cols = this->grid[this->rows].size();
            this->rows++;
        }

        Forest(){
            this->visible_trees = 0;
            this->rows = 0;
            this->cols = 0;
            this->most_scenic = NULL;
        }
        ~Forest(){};
};

int main(){
    //Init values / open file
    std::string filename = "8/input.txt";
    std::ifstream file_stream(filename,std::ios::in);
    std::string line; 

    Forest our_forest;

    //Parse file
    while(file_stream){
        getline(file_stream,line);
        if(line.empty() || line[0] == '\n'){
            continue;
        }
        //feed in line of trees one row at a time.
        our_forest.parseLineOfTrees(line);
    }

    //Find all visible trees and populate scenic scores
    our_forest.findVisibleTrees();
    //Do calculations to find the visible count/most scenic
    our_forest.calculateVisibleTrees();

    std::cout << "There are " << our_forest.visible_trees << " visible trees within the forest (part1).\n";
    std::cout << "The tree with the best scenery count is " << our_forest.most_scenic->getCoords() << " with a score of " << our_forest.most_scenic->scenic_score << " (part2).\n";

    return 0;
}
