/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description: Randomly fills an array of size 10x10 with 0s and 1s, and      *
 *              outputs the number of blocks in the largest block              *
 *              construction, determined by rows of 1s that can be stacked     *
 *              on top of each other.                                          *
 *                                                                             *
 * Written by Eric Martin and *** for COMP9021                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define DIM 4

bool grid[DIM][DIM];

void display_grid(void);
int size_of_largest_construction(void);
/* If j1 <= j2 and the grid has a 1 at the intersection of row i and column j
 * for all j in {j1, ..., j2}, then returns the number of blocks in the construction
 * built over this line of blocks. */
int construction_size(int i, int j1, int j2);

int main(int argc, char **argv) {
    /* We run the program with two command line arguments. */
    if (argc != 3) {
        printf("Please provide 2 command line arguments.\n");
        return EXIT_FAILURE;
    }
    /* The first one will determine our probability distribution;
     * it is meant to be strictly positive. */
    int nb_of_possible_outcomes = strtol(argv[1], NULL, 10);
    /* We use the second command line argument, meant to be a nonnegative integer,
     * as a seed for the random number generator. */
    srand(strtoul(argv[2], NULL, 10));
    /* We fill the grid with randomly generated 0s and 1s,
     * with for every cell, a probability of 1/nb_of_possible_outcomes to generate a 0. */
    for (int i = 0; i < DIM; ++i)
        for (int j = 0; j < DIM; ++j)
            grid[i][j] = rand() % nb_of_possible_outcomes;
    puts("Here is the grid that has been generated:\n");
    display_grid();
    printf("The largest block construction has %d blocks.\n", size_of_largest_construction());    
    return EXIT_SUCCESS;
}

void display_grid(void) {
    for (int i = 0; i < DIM; ++i) {
        printf("    ");
        for (int j = 0; j < DIM; ++j)
            grid[i][j] ? printf(" 1") : printf(" 0");
        putchar('\n');
    }
    putchar('\n');
}

int size_of_largest_construction(void) {
    int largest_block_size = 0, counter = 0;
    for (int i = DIM - 1; i >= 0; i--) 
        for (int j = 0; j < DIM; j++) {
            if (grid[i][j]) {
                if (j == DIM - 1) {
                    int size_found = construction_size(i, j - counter, j) + j + 1;
                    largest_block_size = (largest_block_size < size_found) ? size_found : largest_block_size;
                    counter = 0;
                    break;
                } 
                counter++;
            }
            else if (counter) {
                int size_found = construction_size(i, j - counter, j - 1) + j - 1;
                largest_block_size = (largest_block_size < size_found) ? size_found : largest_block_size;
                counter = 0;
            }
        }
    return largest_block_size;
}

int construction_size(int i, int j1, int j2) {
    int counter = 0, size = 0;
    if (i) { 
        for (; j1 <= j2; j1++) {
            if (grid[i - 1][j1]) {
                counter++;
                if (j1 == j2 && counter)
                   size = counter + construction_size(i - 1, j1 - counter + 1, j1); 
            }
            else if (counter) 
                size = counter + construction_size(i - 1, j1 - counter, j1 - 1);
        } 
        if (counter == 0) 
            return size;
    }
    else
        return size;
}

            

