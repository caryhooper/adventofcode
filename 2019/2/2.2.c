#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>    
#include <strings.h>    //bzero

//gcc -g 2.1.c -o part1.elf

#define MAX_LONG_LENGTH 20
#define CODE_MAX 256

/**
 * @brief - parses a comma-separated file of integers to be placed into a C array
 * 
 * @param file - pointer to input file to be parsed
 * @param machine_code - array of integers to be populated file
 */
void parse_file(FILE * file, unsigned int machine_code[CODE_MAX]){
    char next_num_string[20];
    bzero(next_num_string,20);
    unsigned int next_num_int = 0;

    char next_char;
    int index = 0;
    while( (next_char = fgetc(file)) != EOF ){
        if(next_char == ','){
            sscanf(next_num_string,"%u",&next_num_int);
            machine_code[index] = next_num_int;
            bzero(next_num_string,20);
            index++;
            continue;
        }
        strncat(next_num_string,&next_char,1);
    }
    sscanf(next_num_string,"%u",&next_num_int);
    machine_code[index] = next_num_int;
}

void run_program(unsigned int machine_code[CODE_MAX]){
    int eip = 0;
    while(1){
        //printf("first position is %u\n",machine_code[0]);
        unsigned int instruction = machine_code[eip];
        unsigned int position1 = machine_code[eip+1];    
        unsigned int position2 = machine_code[eip+2];
        unsigned int result_position = machine_code[eip+3];
        if (instruction == 99){
            return;
        } else if (instruction == 1){
            //addition
            machine_code[result_position] = machine_code[position1] + machine_code[position2];
        } else if (instruction == 2){
            //multiplication
            machine_code[result_position] = machine_code[position1] * machine_code[position2];
        } else {
            printf("Error. Current command is %u but expected 1, 2, or 99.",instruction);
            exit(1);
        }
        eip+=4;
    }
}

// void reset_memory(unsigned int machine_code[CODE_MAX], unsigned int machine_code_copy[CODE_MAX]){
//     for(int i = 0; i < CODE_MAX; i++){
//         machine_code_copy[i] = machine_code[i];
//     }
// }
//Position 0 - 1 (ADD BY REF) ,2 (MULTIPLY BY REF),99 (HALT)
//Move forward 4 positions.

int main(){
    const char * filename = "2/input.txt";
    FILE * input_file = fopen(filename,"r");
    if(input_file == NULL){
        printf("Failure to open file.");
        exit(1);
    }

    //Build instruction array by parsing file.
    unsigned int machine_code[CODE_MAX];
    parse_file(input_file,machine_code);

    unsigned int output = 0;
    int noun = 0; 
    int verb = 0;
    
    for(int i = 0; i < 100; i++){
        for(int j = 0; j < 100; j++){
            unsigned int machine_code_copy[CODE_MAX];
            memcpy(&machine_code_copy[0],&machine_code[0],sizeof(machine_code));

            machine_code_copy[1] = i;
            machine_code_copy[2] = j;
            run_program(machine_code_copy);
            output = machine_code_copy[0];
            if(output == 19690720){
                noun = i;
                verb = j;
                i = j = 100;
                printf("The two inputs resulting in 19690720 for Day 2 Part 2 is %u and %u.\n",noun,verb);
                break;
            }
        }
    }
    unsigned int solution = (100 * noun) + verb;
    printf("The answer is %u\n",solution);
}