#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>       //floor
#include <strings.h>    //bzero

//gcc -g 1.1.c -o part1.elf


#define MAX_LONG_LENGTH 20

unsigned long calculateFuelFromModule(unsigned long mass){
    return (unsigned long) floor(mass / 3) - 2;

}

int main(){
    const char * filename = "input.txt";
    FILE * input_file = fopen(filename,"r");

    char buffer[MAX_LONG_LENGTH];
    bzero(&buffer[0],MAX_LONG_LENGTH);
    unsigned long mass_value;
    unsigned long solution = 0; //sum of all fuel needed for each module mass

    while( fgets(&buffer[0],MAX_LONG_LENGTH,input_file) != NULL ) {
        sscanf(buffer,"%lu",&mass_value);

        solution += calculateFuelFromModule(mass_value);

        bzero(&buffer[0],MAX_LONG_LENGTH);
    }
    printf("The solution for Day 1 Part 1 is %lu of fuel.\n",solution);
}