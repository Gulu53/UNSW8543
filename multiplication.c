/*
 * =====================================================================================
 *
 *       Filename:  multiplication.c
 *
 *    Description:  Output the multiplication in the required format with found solutions
 *
 *        Version:  1.0
 *        Created:  13/08/13 17:27:04
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  NilH 
 *   Organization:  UNSW 
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_PRO 9988
#define MIN_PRO 1100
#define MAX_PAR_ONE 8988
#define MIN_PAR_ONE 2100
#define MAX_PAR_TWO 898
#define MIN_PAR_TWO 210

int main(void) {
    for (int m = 100; m < 1000; m += 2) {
        if (!(m / 100 % 2)) {
            /*Skip the next 100 numbers 98 + 2 = 100*/
            m += 98;
            continue;
        }
        if (m / 10 % 10 % 2) {
            /*Skip the next 10 numbers 8 + 2 = 10*/
            m += 8;
            continue;
        }
        if (m % 10 % 2)
          continue;  
        for (int n = 20; n < 100; n +=2) {        
            if (n / 10 % 10 % 2) {
                n += 8;
                continue;
            }
            if (n % 10 % 2)
                continue;
            int product = m * n;
            if (product < MIN_PRO || product > MAX_PRO)
                continue;
            if (product % 10 % 2 || product / 10 % 10 % 2)
                continue;
            if (!(product / 100 % 10 % 2) || !(product / 1000 % 2)) 
                continue; 
            //printf("%d\n",product);
            int partial_prd_one = m * (n % 10);
            if (partial_prd_one < MIN_PAR_ONE || partial_prd_one > MAX_PAR_ONE)
                continue;
            if (partial_prd_one / 1000 % 2 || partial_prd_one / 10 % 10 % 2 || partial_prd_one % 10 % 2)
                continue;
            if (!(partial_prd_one / 100 % 10 % 2))
                continue;
            int partial_prd_two = m * (n / 10 % 10);
            if (partial_prd_two < MIN_PAR_TWO || partial_prd_two > MAX_PAR_TWO)
                continue;
            if (partial_prd_two / 100 % 10 % 2 || partial_prd_two % 10 % 2)
                continue;
            if (!(partial_prd_two / 10 % 10 % 2))
                continue;
            printf("%4d\nx%3d\n", m, n);
            printf("----\n");
            printf("%4d\n%3d \n", partial_prd_one, partial_prd_two);
            printf("----\n");
            printf("%4d\n",product);
        }
    }    
}
