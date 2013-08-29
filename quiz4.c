/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description: Reads a line of input and transforms it in some way...         *
 *                                                                             *
 * Written by Eric Martin and NilH for COMP9021                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define UPPER_CASE 1
#define LOWER_CASE 2
#define NUMBER_BASE 48
#define SPACE 32
#define NUMBER_ROOF 57
#define UPPER_Z 90
#define LOWER_Z 122
#define CHAR_BIAS 26


int main(void) {
    int last_digit_seen = 0;
    /* To start with, we pretend that we have seen a letter
     * that is both uppercase and lowercase.
     * So the first time we see a letter, we can guarantee that it is
     * of the same type as the last letter that we (pretend we have) seen
     * and we do not print out a space. */
    int last_case_seen = UPPER_CASE + LOWER_CASE;
    int c;
    int output_bias = 0;
    while ((c = getchar()) != '\n') {
        if (!isalnum(c))
            continue;
        if (c >= NUMBER_BASE && c <= NUMBER_ROOF) {
            output_bias = c - NUMBER_BASE;
            continue;
        }
        if (isupper(c)) {
            (!(last_case_seen == LOWER_CASE)) ? : putchar(SPACE);
            (c + output_bias > UPPER_Z) ? putchar(c + output_bias - CHAR_BIAS) : putchar(c + output_bias);
            last_case_seen = UPPER_CASE;
        }
        else if (islower(c)) { 
            (!(last_case_seen == UPPER_CASE)) ? : putchar(SPACE);
            (c + output_bias > LOWER_Z) ? putchar(c + output_bias - CHAR_BIAS) : putchar(c + output_bias);
            last_case_seen = LOWER_CASE;
        }
    }
    putchar('\n');
    return EXIT_SUCCESS;
}

