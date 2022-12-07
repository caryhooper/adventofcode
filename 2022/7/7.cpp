#include <bits/stdc++.h>    //std::stoi, size_t
#include <ctype.h>

//File object used here like a struct.
class File{
    public:
        size_t size;
        std::string name;
        File(size_t size, std::string name){
            this->size = size;
            this->name = name;
        }
        ~File(){};
};

//Container for Files and other Directories.  Keeps track of size of all subfiles/directories.
class Directory{        
    public:
        std::string name;
        size_t size;
        Directory * parent_dir;
        std::vector<Directory *> subdirectories;
        std::vector<File> files;

        Directory * getSubdirectoryByName(std::string dir_name){
            for(size_t i = 0; i < this->subdirectories.size(); i++){
                if(dir_name.compare(this->subdirectories[i]->name) == 0){
                    return this->subdirectories[i];
                }
            }
            std::cout << "Warning... directory " << dir_name << " NOT FOUND\n";
            return NULL;
        }

        //Called initially. Recursive function dives into each subdirectory and calculates the size of all files and subdirectories
        //  Contained within.  
        size_t getDirectorySize(){
            size_t total = 0;
            //Add up directories. 
            for(size_t i = 0; i < this->subdirectories.size(); i++){
                size_t dir_size = 0;
                //If a subdirectory's size is not set... call getDirectorySize to find the total.
                if(this->subdirectories[i]->size == 0){
                    dir_size = this->subdirectories[i]->getDirectorySize();
                } else {
                    //If it is already set, just get it from the size attribute.
                    dir_size = this->subdirectories[i]->size;
                }
                std::cout << "The size of " << this->subdirectories[i]->name << " is " << dir_size << "\n";
                total += dir_size;
            }
            //Add up files
            for(size_t i = 0; i < this->files.size(); i++){
                    total += this->files[i].size;
            }
            //update entire directory size.
            this->size = total;
            return total;
        }

        //Part 1
        size_t getDirectoriesSizeUnderMax(size_t max_size){
            //Directory size includes files and sum of directories underneath it.  
            size_t total = 0;
            //Iterate over all subdirectories
            for(size_t i = 0; i < this->subdirectories.size(); i++){
                size_t dir_size = this->subdirectories[i]->size;
                //std::cout << "Checked directory: " << this->name << " | size is " << dir_size << "\n";
                //If the subdirectory size is less than the given value, add it to the total
                if(dir_size <= max_size){
                    total += dir_size;
                }
                //Also add up sizes of its subdirectories under the given value
                size_t subdirectory_total = this->subdirectories[i]->getDirectoriesSizeUnderMax(max_size);
                total += subdirectory_total;
            }
            return total;
        }

        //Part 2. 
        //Finds the smallest directory greater than the threshold min_size.
        size_t findDirectoryToDelete(size_t min_size, size_t min_size_so_far){
            size_t subdirectories_num = this->subdirectories.size();
            //std::cout << this->name << " contains " << subdirectories_num << " subdirectories.\n";
            for(size_t i = 0; i < subdirectories_num; i++){
                size_t dir_size = this->subdirectories[i]->size;
                if( (dir_size > min_size) && (dir_size < min_size_so_far)){
                    min_size_so_far = dir_size;
                }
                //Do the same with this subdirectory.  Recursive programming pattern.
                min_size_so_far = this->subdirectories[i]->findDirectoryToDelete(min_size,min_size_so_far);
            }
            return min_size_so_far;
        }

        //Helper function for printTree for indents.
        void printTab(int level){
            for(int i = 0; i < level; i++){
                std::cout << "    ";
            }
        }

        //Prints a tree-like structure for directory / file visualization.
        void printTree(int level = 0){
            if(level == 0){
                std::cout << "- " << this->name << " (dir)\n";
            }
            level++;
            for(size_t i = 0; i < this->files.size(); i++){
                this->printTab(level);
                std::cout << "- " << this->files[i].name << " (file, size=" << this->files[i].size << ")\n";
            }

            for(size_t i = 0; i < this->subdirectories.size(); i++){
                this->printTab(level);
                std::cout << "- " << this->subdirectories[i]->name << " (dir)\n";
                level++;
                this->subdirectories[i]->printTree(level);
                level--;
            }
        }

        Directory(std::string name, Directory * parent_dir = NULL){
            this->name = name;
            this->parent_dir = parent_dir;
            this->size = 0;
        };
        ~Directory(){
            for(size_t i = 0; i < this->subdirectories.size(); i++){
                delete (this->subdirectories[i]);
            }
        };
};


//Pass in a string by reference to parse by a ' ' character.  Returns all characters up to the ' '
//  and removes that token form the string.
std::string getNextToken(std::string& line){
    auto iter = line.find(' ');
    std::string token = line.substr(0,iter);
    line.erase(0,iter + 1);
    return token;
}

//Helper function to parse all the commandline input into a file structure.
Directory * parseCommandline(std::string filename){
    std::ifstream file_stream(filename,std::ios::in);
    std::string line; 
    Directory * current_directory = NULL;

    while(file_stream){
        getline(file_stream,line);
        if(line.empty()){
            continue;
        }

        //Either:
        //  $ -> command
        //      c -> cd context
        //      l -> list
        //  not $ -> result
        //      d -> subdirectory
        //      0-9 -> file
        if(line[0] == '$'){
            //command
            getNextToken(line);
            std::string command = getNextToken(line);
            if(command[0] == 'c'){
                //cd
                std::string arg = getNextToken(line);

                if(arg.compare("..") == 0 ){
                    //go up a directory
                    current_directory = current_directory->parent_dir;
                } else {
                    if(current_directory == NULL){
                        current_directory =  new Directory(arg);
                    } else {
                        current_directory = current_directory->getSubdirectoryByName(arg);
                    }
                }
            } else if (command[0] == 'l') {
                //ls (nop)
                continue;
            } else {
                std::cout << "Illegal command: " << line << "\n";
            }
        } else {
            //directory of file
            if(line[0] == 'd'){
                //dir
                getNextToken(line);
                // std::string dir_name = getNextToken(line);
                Directory * dir = new Directory(line,current_directory);
                current_directory->subdirectories.push_back(dir);
            } else if (std::isdigit(line[0])){
                //file
                std::string size_s = getNextToken(line);
                // std::string name = getNextToken(line);
                current_directory->files.push_back(File(std::stoul(size_s),line));
            } else {
                std::cout << "Illegal container within directory: " << line << "\n";
            }
        }
    }

    //Walk the tree up to the root directory.
    while(current_directory->parent_dir != NULL){
        current_directory = current_directory->parent_dir;
    }
    return current_directory;
}

//Part1 - find all of the directories with a total size of at most 100000 bytes

// $ cd /
// $ ls
// dir a
// 14848514 b.txt
// 8504156 c.dat
// dir d
// $ cd a
// $ ls
// dir e
// 29116 f
// 2557 g
// 62596 h.lst
// $ cd e
// $ ls
// 584 i
// $ cd ..
// $ cd ..
// $ cd d
// $ ls
// 4060174 j
// 8033020 d.log
// 5626152 d.ext

int main(){
    //Init values / open file
    std::string filename = "7/input.txt";


    //Parse file
    Directory * root_dir = parseCommandline(filename);
    //Initialize the root_dir subdirectory sizes.
    root_dir->getDirectorySize();
    //print just for kicks
    root_dir->printTree();
    std::cout << "The total size of the hard disk is " << root_dir->size << "\n";

    //Part1
    size_t answer = root_dir->getDirectoriesSizeUnderMax(100000);
    std::cout << "The answer for part1 is " << answer << " (part1).\n";

    //Part2
    size_t total_disk_space = 70000000;
    size_t disk_space_needed = 30000000;
    size_t need_to_free = root_dir->size - total_disk_space + disk_space_needed ;
    std::cout << "We need to free " << need_to_free << ".\n";
    size_t size_of_dir_to_delete = root_dir->findDirectoryToDelete(need_to_free,UINT64_MAX);
    std::cout << "The size of the directory we need to delete is " << size_of_dir_to_delete << "\n";

    //Cleanup
    delete root_dir;
    return 0;
}
