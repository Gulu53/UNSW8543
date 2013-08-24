/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description: Prompts the user to input the 32 bits of a normalised          *
 *              floating point number and outputs a decimal representation     *
 *              of that number.                                                *
 *                                                                             *
 * Written by Eric Martin and NilH for COMP9021                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define NB_OF_BITS_FOR_NUMBER 32
#define NB_OF_BITS_FOR_EXPONENT 8
#define NB_OF_BITS_FOR_DECIMAL_PART 23
#define BASE 2

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void complain(void);
void get_mantissa(int *digits, float *mantissa);
void get_expo (int *digits, signed int *expo);

int main(void) {

    printf("Enter the %d bits of a normalised floating point number,\n"
           "      not equal to 0, with bytes in their \"natural\" order,\n"
           "      all on one line (spaces accepted):\n ",
           NB_OF_BITS_FOR_NUMBER);
    int digits[NB_OF_BITS_FOR_NUMBER];
    int c;
    int i = 0;
    const char negative = '-', power = '^';
    signed int exponent  = 0;
    float mantissa = 0;   
    int bias = (int)pow((double)BASE, (double)((NB_OF_BITS_FOR_EXPONENT) - 1)) - 1;
    
    while ((c = getchar()) != '\n') {
        if (isblank(c))
            continue;
        if (c != '0' && c != '1')
            complain();
        digits[i++] = c - '0';
    }
    if (i != NB_OF_BITS_FOR_NUMBER)
        complain();
    puts("This codes the following floating point number:");
    get_mantissa(digits, &mantissa);
    get_expo(digits, &exponent);
    !digits[0] ? : printf("%c  ", negative);
    printf("%d%c%d x %1.24f\n",BASE, power, exponent - bias, mantissa);
    return EXIT_SUCCESS;
}

void complain(void) {
    printf("You did not provide me with what I asked for!\n");
    exit(EXIT_FAILURE);
}

void get_mantissa(int *digits, float *mantissa) {
    int end_loc = NB_OF_BITS_FOR_NUMBER - 1;
    int start_loc = end_loc - NB_OF_BITS_FOR_DECIMAL_PART + 1;
    for (int i = start_loc; i <= end_loc; ++i) {
        static double p = -1;
        if (*(digits + i)) 
            *mantissa += (float)pow((double)(*(digits + i) * 2), p); 
        p--;
    }
    ++*mantissa;
}

void get_expo (int *digits, signed int *expo) {
    int start_loc = 1;
    int end_loc = start_loc + NB_OF_BITS_FOR_EXPONENT - 1;
    for (int i = end_loc; i >= start_loc; i--) {
        static double p = 0;
        if (*(digits + i))
            *expo += (unsigned int)pow((double)(*(digits + i) * 2), p);
        p++;
    }    
}
    
