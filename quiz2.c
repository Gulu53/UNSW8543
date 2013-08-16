/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description: Prompts the user to input a positive number at most equal to   *
 *              2^20 - 1 and outputs the result of reversing the 20 rightmost  *
 *              bits of this number.                                           *
 *                                                                             *
 * Written by Eric Martin and NilH for COMP9021                                *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define LENGTH 20

int reverse(int);
void display_set_coded_by(int);

int main(void) {
    const int max = pow(2, LENGTH) - 1;
    printf("Enter a positive number at most equal to %d: ", max);
    int n;
    if (scanf("%d", &n) != 1 || n < 0 || n > max) {
        printf("Incorrect input, stopping there.\n");
        return EXIT_FAILURE;
    }
    printf("Reversing the 20 bits of the number coded as ");
    display_set_coded_by(n);
    int m = reverse(n);
    printf("\n                                      yields ");
    display_set_coded_by(m);
    printf("\nwhich codes the number %d.\n", m);
    return EXIT_SUCCESS;
}

void display_set_coded_by(int n) {
    for (int i = LENGTH - 1; i >= 0; --i)
        if (n & 1 << i)
            putchar('1');
        else
            putchar('0');
}

int reverse(int n) {
    unsigned int top_half_ans = 0, bottom_half_ans = 0; 
    int a_series_end = LENGTH % 2 ? LENGTH / 2 + 1 : LENGTH / 2;
    int b_series_start = LENGTH / 2 + 1; 
    printf("\n");
    for (int i = 0; i < a_series_end; i++) {
        int recorder = 1 << i & n ? 1 : 0;
        top_half_ans |= recorder << (LENGTH - i) - 1;
    }  
    for (int i = b_series_start; i <= LENGTH; i++) {
        int recorder = (1 << i -1) & n ? 1 : 0;
        bottom_half_ans |= recorder << (LENGTH - i);
    }
//    display_set_coded_by(top_half_ans);
//    printf("\n");
//    display_set_coded_by(bottom_half_ans);
    return top_half_ans | bottom_half_ans;
}

    
    
    
    
