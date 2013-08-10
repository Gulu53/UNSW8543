/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description: Finds the length of the longest increasing sequence in a       *
 *              randomly generated sequence of 30 numbers between 0 and 100,   *
 *              as well as the first and last numbers in the last occurrence   *
 *              of such a sequence.                                            *
 *                                                                             *
 * Written by Eric Martin and NilH for COMP9021                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 30
#define BOUND 100

//Code written by NilH on 08/08/13
typedef struct {
    int starting_location, end_location, len;
} SequenceInfo;

void check_sequence(SequenceInfo*, SequenceInfo*);
//END

int main(int argc, char **argv) {

    /* We run the program with one number as command line argument. */
    if (argc != 2) {
        printf("Please provide 1 command line argument.\n");
        return EXIT_FAILURE;
    }
    /* We use it as a seed for the random number generator. */
    srand(strtoul(argv[1], NULL, 10));
    int data[SIZE];
    
    /* We randomly generate 10 numbers in the range [0, 100) and store them in data. */
    for (int i = 0; i < SIZE; ++i)
        data[i] = rand() % BOUND;
    int longest_length = 1, current_length = 1;
    int smallest_number, largest_number;
    
    //Code written by NilH on 08/08/13   
    SequenceInfo chosen_sequence = {0, 0, 0};
    SequenceInfo sequence_found = {0, 0, 0};
    bool new_sequence = true;

    for (int i =1; i < SIZE; i++) {
        if (new_sequence) {             
            sequence_found.starting_location = i - 1;
            new_sequence = false;
        }
        if (!(data[i] > data[i-1])) { 
            sequence_found.end_location = i - 1;
            sequence_found.len = i - sequence_found.starting_location;
            check_sequence(&chosen_sequence, &sequence_found);
            new_sequence =  true;
            if (i == SIZE-1) {
                sequence_found.starting_location = i;
                sequence_found.end_location = i;
                sequence_found.len = 1;
                check_sequence(&chosen_sequence, &sequence_found);
            } 
        }
        else {
            if (i == SIZE -1) {
                sequence_found.end_location = i;
                sequence_found.len = i - sequence_found.starting_location;
                check_sequence(&chosen_sequence, &sequence_found);
                break;
            }
        }
    }
    smallest_number = data[chosen_sequence.starting_location];
    largest_number = data[chosen_sequence.end_location];
    longest_length = chosen_sequence.len;
    //END 
    for (int i = 0; i < SIZE / 3; ++i)
        printf("%4d", data[i]);
    printf("\n ");
    for (int i = SIZE / 3; i < SIZE / 3 * 2; ++i)
        printf("%4d", data[i]);
    printf("\n  ");
    for (int i = SIZE / 3 * 2; i < SIZE; ++i)
        printf("%4d", data[i]);
    printf("\n has a longest stricly increasing sequence of length %d,\n", longest_length);
    printf(" the last of which starts with %d and ends with %d.\n", smallest_number, largest_number);
    return EXIT_SUCCESS;
}
//Code written by NilH on 09/08/13
void check_sequence(SequenceInfo *chosen, SequenceInfo *found ) {
    if (chosen->len < found->len) 
        *chosen = *found;
    else if (chosen->len == found->len) {
        if (chosen->starting_location < found->starting_location)
            *chosen = *found;
    }
}
//END
