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
/*The least possible value of grape is 10234 and max is 98765*/
#define MIN_GRP 10234
#define MAX_GRP 98765

                       
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
        for (int j = MIN_GRP; j <= MAX_GRP; ++j) {
            int digits_org_grp[10];
            for (int ja = 0; ja < 10; ++ja)
                digits_org_grp[ja] = digits_org[ja]; 
            int p = j / 10 % 10;
            if (p == 0 || ++digits_org_grp[p] == 2)
                continue;
            int b = ((o * 100000 + r * 10000 + a * 1000 + n * 100 + g * 10 + e) - (g * 10000 + r * 1000 + a * 100 + p * 10 + e) - (p * 1000 + e * 100 + a * 10 + r)) / 10000;
            if (b == 0 || ++digits_org_grp[b] == 2) 
                continue;
            else {
                int orange = o * 100000 + r * 10000 + a * 1000 + n * 100 + g * 10 + e;
                int grape = g * 10000 + r * 1000 + a * 100 + p * 10 + e;
                int pear = p * 1000 + e * 100 + a * 10 + r;
                int banana = b * 100000 + a * 10000 + n * 1000 + a * 100 + n * 10 + a;
                printf("banana = %d, grape = %d, pear = %d and orange = %d is a solution", banana, grape, pear, orange);
            }
        }
    }
}

