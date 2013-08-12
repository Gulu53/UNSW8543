/*
 * =====================================================================================
 *
 *       Filename:  cryptarithm.c
 *
 *    Description:  Solve puzzle banana + grape + pear = orange
 *
 *        Version:  1.0
 *        Created:  12/08/13 20:41:42
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  NilH 
 *   Organization:  UNSW 
 *
 * =====================================================================================
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*The least possible value of organce is 102345 and max is 987654*/
#define MIN_ORG 102345
#define MAX_ORG 987654
/*The least possible value of p is 1 and max is 9*/
#define MIN_P 1
#define MAX_P 9

                       
int main(void) {
    for (int i = MIN_ORG; i <= MAX_ORG; ++i) {
        int digits_org[10] = {0};
        int o = i / 100000;
        digits_org[o] = 1;
        int r = i / 10000 % 10;
        if (++digits_org[r] == 2)
            continue;
        int a = i / 1000 % 10;
        if (++digits_org[a] == 2)
            continue;
        int n = i / 100 % 10;
        if (++digits_org[n] == 2)
            continue;
        int g = i / 10 % 10;
        if (g == 0 || ++digits_org[g] == 2)
            continue;
        int e = i % 10;
        if (++digits_org[e] == 2)
            continue;
        for (int j = MIN_P; j <= MAX_P; ++j) {
            if (digits_org[j] + 1 == 2)
                continue;
            int p = j;
            digits_org[p] = 1;
            int b = ((o * 100000 + r * 10000 + a * 1000 + n * 100 + g * 10 + e) - (g * 10000 + r * 1000 + a * 100 + p * 10 + e) - (p * 1000 + e * 100 + a * 10 + r)) / 100000;
            if (b == 0 || digits_org[b] + 1 == 2) { 
                digits_org[p] = 0;
                continue;
            }
            int orange = o * 100000 + r * 10000 + a * 1000 + n * 100 + g * 10 + e;
            int grape = g * 10000 + r * 1000 + a * 100 + p * 10 + e;
            int pear = p * 1000 + e * 100 + a * 10 + r;
            int banana = b * 100000 + a * 10000 + n * 1000 + a * 100 + n * 10 + a;
            if (banana + pear + grape == orange) {
                printf("banana = %d, grape = %d, pear = %d and orange = %d is a solution.", banana, grape, pear, orange);
                return EXIT_SUCCESS;
            }
            digits_org[p] = 0;
        }
    }
    return EXIT_SUCCESS;
}

