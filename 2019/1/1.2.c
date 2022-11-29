#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>       //floor
#include <strings.h>    //bzero

//gcc -g 1.1.c -o part1.elf


#define MAX_LONG_LENGTH 20

//Recursive function 
unsigned long calculateTotalFuel(unsigned long mass){
    // unsigned long fuel_remaining = mass;
    unsigned long total_fuel_required = 0;
    if(mass > 6){
        total_fuel_required += (unsigned long) floor(mass / 3) - 2;
        total_fuel_required += calculateTotalFuel(total_fuel_required);
    }
    return total_fuel_required;
}

int main(){
    const char * filename = "1/input.txt";
    FILE * input_file = fopen(filename,"r");
    if(input_file == NULL){
        exit(1);
    }

    char buffer[MAX_LONG_LENGTH];
    bzero(&buffer[0],MAX_LONG_LENGTH);
    unsigned long mass_value;
    unsigned long solution = 0; //sum of all fuel needed for each module mass

    while( ( fgets(&buffer[0],MAX_LONG_LENGTH,input_file)) != NULL ) {
        sscanf(buffer,"%lu",&mass_value);

        unsigned long fuel_for_fuel = calculateTotalFuel(mass_value);
        solution += fuel_for_fuel;

        bzero(&buffer[0],MAX_LONG_LENGTH);
    }
    printf("The solution for Day 1 Part 2 is %lu of fuel.\n",solution);
}