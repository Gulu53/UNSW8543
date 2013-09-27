/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Description:                                                                *
 *                                                                             *
 * Written by Kim for COMP9021                                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MIN_WIDTH 2
#define MAX_WIDTH 50
#define MIN_HEIGHT 2
#define MAX_HEIGHT 50
#define TO_SCALE 0.4
#define TOTAL_NUMBER_OF_DIR 8
#define N 0
#define NE 1
#define E 2
#define SE 3
#define S 4
#define SW 5
#define W 6
#define NW 7
#define GET_ID(X) (X & 0b0000111111111111)
#define GET_DIRECTION(X) ((X >> 12) & 0b0111)
#define PAINT(DIRECTION, ID) ((DIRECTION << 12) + ID)

signed short int input_data[MAX_WIDTH][MAX_HEIGHT];
signed short int offset[8][2] = {{-1, 0},{-1, 1},{0, 1},{1, 1},{1, 0},{1, -1},{0, -1},{-1, -1}};

bool get_input(void);
bool get_polygon(int, int, int, int, int);
signed int get_neighbour(int current_x, int current_y, int polygon_id);
void print_perimeter(int x, int y);
float get_area(int x, int y);
bool check_convex(int x, int y);
unsigned int get_rotations(int x, int y);
unsigned int get_depth(int x, int y);
void print_matrix(void); 

int main(int argc, char **argv) {
    if (argc > 2 || argc == 2 && strcmp(argv[1], "print")) {
        printf("I expect no command line argument or \"print\" as unique command line argument.\n");
        return EXIT_FAILURE;
    }
    if (!get_input()) {
        printf("Incorrect input.\n");
        return EXIT_FAILURE;
    }
    if (argc == 1) {
        signed int last_polygon = 1;
        for (int y = 0; y < MAX_HEIGHT; y++)
            for (int x = 0; x < MAX_WIDTH; x++) {
                if (input_data[y][x] < 0)
                    break;
                if (GET_ID(input_data[y][x]) > last_polygon) {
                    printf("Polygon %d:\n", GET_ID(input_data[y][x]) - 1);
                    printf("    Perimeter: ");
                    print_perimeter(x, y);
                    printf("    Area: %4.2f\n", get_area(x, y));
                    printf("    Convex: %s\n", (check_convex(x, y) ? "yes" : "no"));
                    printf("    Nb of invariant rotations: %d\n", get_rotations(x, y));
                    printf("    Depth: %d\n", get_depth(x, y));
                    last_polygon++;
                }
            }
    }
    else {
        /* Insert your code for the case a.out is run with print as command line argument */
    }
    return EXIT_SUCCESS;
}
bool get_input(void) {
    int input_char = 0, expected_width = 0, x_write_loc = 0, y_write_loc = 0;
    for (int y = 0; y < MAX_HEIGHT; y++)
        for (int x = 0; x < MAX_WIDTH; x++)
            input_data[y][x] = -1;
    while ((input_char = getchar()) != EOF) {
        if (input_char == '\n') {
            if (x_write_loc == 0)
                continue;
            if (x_write_loc < MIN_WIDTH)
                return false;
            y_write_loc++; 
            x_write_loc = 0;
            continue;
        } 
        else if (isspace(input_char))
            continue;
        else if ((input_char - '0' != 0) && (input_char - '0' != 1)) 
            return false;
        else {        
            input_data[y_write_loc][x_write_loc] = (signed short int)(input_char - '0'); 
            x_write_loc++;
        }
        if ((x_write_loc > MAX_WIDTH) || (y_write_loc > MAX_HEIGHT))
            return false;
    }
    if (y_write_loc < MIN_HEIGHT)
        return false;
    int polygon_id = 2; 
    for (int y = 0; y < MAX_HEIGHT; y++)
        for (int x = 0; x < MAX_WIDTH; x++) {
            if (input_data[y][x] < 0)
                break;
            if (input_data[y][x] == 1) {
                bool ret = get_polygon(x, y, x, y, polygon_id);
                if (ret) 
                    polygon_id++; 
                else
                    return false;
            }
        }
    return true;    
}
bool get_polygon(int org_x, int org_y, int current_x, int current_y, int polygon_id) {
    int coming_from = 0, direction_offset = 1, current_id = GET_ID(input_data[current_y][current_x]); 
    if ((GET_ID(input_data[org_y][org_x]) == polygon_id) && (org_x == current_x) && (org_y == current_y)) 
        return true;
    if (current_id != 1)
        return false; 
    if ((coming_from = get_neighbour(current_x, current_y, polygon_id)) < 0) {
        direction_offset = 0;
        coming_from = 0;
    }
    int start_direction = (coming_from + 1 > 7) ? N : (coming_from + 1);
    int current_direction = start_direction;
    for (int i = 1; i <= (TOTAL_NUMBER_OF_DIR - direction_offset); i++) {
        if ((0 > (current_x + offset[current_direction][1])) || ((current_x + offset[current_direction][1]) >= MAX_WIDTH)) { 
            current_direction = (current_direction + 1 > NW) ? 0 : current_direction + 1;                                    
            continue;
        }
        if ((0 > (current_y + offset[current_direction][0])) || ((current_y + offset[current_direction][0]) >= MAX_HEIGHT)) {
            current_direction = (current_direction + 1 > NW) ? 0 : current_direction + 1;                                    
            continue;
        }
        input_data[current_y][current_x] = PAINT(current_direction, polygon_id);
        if (get_polygon(org_x, org_y, (current_x + offset[current_direction][1]), (current_y + offset[current_direction][0]), polygon_id))
            return true;
        else
            input_data[current_y][current_x] = 1;
        current_direction = (current_direction + 1 > NW) ? 0 : current_direction + 1;                                    
    }
    return false;
}
signed int get_neighbour(int current_x, int current_y, int polygon_id) {
    for (int i = 0; i < TOTAL_NUMBER_OF_DIR; i++) { 
        if ((0 > (current_x + offset[i][1])) || ((current_x + offset[i][1]) >= MAX_WIDTH)) 
            continue;
        if ((0 > (current_y + offset[i][0])) || ((current_y + offset[i][0]) >= MAX_HEIGHT))
            continue;
        if (GET_ID(input_data[current_y + offset[i][0]][current_x + offset[i][1]]) == polygon_id) {
            int pre_direction = GET_DIRECTION(input_data[current_y + offset[i][0]][current_x + offset[i][1]]);
            if (((pre_direction + 4 > NW) ? (pre_direction + 4 - 8) : (pre_direction + 4)) == i)
               return i; 
        }
    }  
    return -1; 
}
void print_perimeter(int x, int y) {
    int current_x = -1, current_y = -1;
    float a = 0, b = 0;
    bool init = true;
    char *plus = " + ", *pending = "*sqrt(.32)";
    static char perimeter[21];
    while ((current_x != x) || (current_y != y)) {
        int direction = -1;
        if (init) {
            init = false;
            current_x = x;
            current_y = y;
        }
        direction = GET_DIRECTION(input_data[current_y][current_x]);
        (direction % 2) ? b++ : a++;
        current_x += offset[direction][1];
        current_y += offset[direction][0];
    }
    if (a)  
        printf("%.1f", a * TO_SCALE);
    if (b) {
        if (a)
            printf("%s", plus);
        printf("%d%s", (int)b, pending);
    }
    putchar('\n');
    return ;
}
float get_area(int x, int y) {
    return 1.0;
}
bool check_convex(int x, int y) {
    return false;
}
unsigned int get_rotations(int x, int y) {
    return 1;
}
unsigned int get_depth(int x, int y) {
    return 1;
}
void print_matrix(void) {
    for (int y = 0; y < MAX_HEIGHT; y++) {
        bool nothing = true;
        for (int x = 0; x < MAX_WIDTH; x++) {
            if (input_data[y][x] >= 0) {
                nothing = false;
                printf("%d ", GET_ID(input_data[y][x]));
            }
            else 
                break;
        } 
        putchar('\n');
        if (nothing)
            break;
    }
}

