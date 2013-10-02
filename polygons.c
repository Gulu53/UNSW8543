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

typedef struct {int x; int y;}Point;

signed short int input_data[MAX_WIDTH][MAX_HEIGHT];
signed short int offset[8][2] = {{-1, 0},{-1, 1},{0, 1},{1, 1},{1, 0},{1, -1},{0, -1},{-1, -1}};

bool get_input(void);
bool get_polygon(int, int, int, int, int);
signed int get_neighbour(int current_x, int current_y, int polygon_id);
void print_perimeter(int x, int y);
float get_area(int x, int y);
float get_segment_area (Point, Point); 
void find_rising_edge(Point* falling_start, Point* falling_end, Point* rising_start, Point* rising_end); 
void find_rising_end(Point* falling_start, Point* falling_end, Point* rising_start, Point* rising_end); 
void reset_falling_end(Point* falling_start, Point* falling_end, Point* rising_start, Point* rising_end); 
bool check_convex(int x, int y);
bool check_vertexes(int org_direction, int future_direction, int vertex_x, int vertex_y);
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
                    printf("    Area: %.2f\n", get_area(x, y));
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
    Point falling_start, falling_end; 
    float total_area = 0;
    int current_x = -1, current_y = -1, line_gradient = -1;
    bool init = true, line_start = false;
    while ((current_x != x) || (current_y != y)) {
        int direction = -1;
        if (init) {
            init = false;
            current_x = x;
            current_y = y;
        } 
        direction = GET_DIRECTION(input_data[current_y][current_x]);
        if ((direction > E) && (direction < W) && (!(line_start))) {
            line_start = true;
            falling_start.x = current_x;
            falling_start.y = current_y;
            line_gradient = direction;
        } 
        if ((line_start) && (direction != line_gradient)){
            line_start = false;
            falling_end.x = current_x;
            falling_end.y = current_y;
            total_area += get_segment_area(falling_start, falling_end);
        }
        else { 
            current_x += offset[direction][1];
            current_y += offset[direction][0];
        }
    }
    return total_area;
}
float get_segment_area (Point falling_start, Point falling_end) {
    int direction = -1, id = -1;
    Point rising_start = falling_end, rising_end = falling_end;
    float accumulated_area = 0, 
          total_area = ((falling_end.x + falling_start.x) * TO_SCALE) * ((falling_end.y - falling_start.y) * TO_SCALE) * 0.5;     
    while (rising_end.y != falling_start.y) {
        rising_start = falling_end;
        rising_end = falling_end;
        find_rising_edge(&falling_start, &falling_end, &rising_start, &rising_end); 
        accumulated_area += (((rising_end.x + rising_start.x) * TO_SCALE) * ((rising_start.y - rising_end.y) * TO_SCALE)) * 0.5;  
    }
    return  total_area - accumulated_area; 
}
void find_rising_edge(Point* falling_start, Point* falling_end, Point* rising_start, Point* rising_end) { 
    while (rising_start->x >= 0) {    
        int id = GET_ID(input_data[rising_start->y][rising_start->x]);
        int direction = GET_DIRECTION(input_data[rising_start->y][rising_start->x]); 
        if ((rising_start->x == falling_end->x) && (rising_start->y == falling_end->y)) { 
            if ((direction == N) || (direction == NW)) {
                *(rising_end) = *(rising_start);     
                find_rising_end(falling_start, falling_end, rising_start, rising_end);
                break;
            }
        }
        else if ((id == GET_ID(input_data[falling_end->y][falling_end->x])) && ((direction > W) || (direction < E))) {
            *(rising_end) = *(rising_start);           
            find_rising_end(falling_start, falling_end, rising_start, rising_end);
            break;
        }
        rising_start->x += offset[W][1];
        rising_start->y += offset[W][0];
    }
}
void find_rising_end(Point* falling_start, Point* falling_end, Point* rising_start, Point* rising_end) {
    int direction = GET_DIRECTION(input_data[rising_start->y][rising_start->x]);
    bool reset = false;
    while (rising_end->y != falling_start->y) {
        if (GET_DIRECTION(input_data[rising_end->y][rising_end->x]) != direction) {
            reset_falling_end(falling_start, falling_end, rising_start, rising_end);
            reset = true;
            break;  
        }
        else {
            rising_end->x += offset[direction][1];
            rising_end->y += offset[direction][0];
        }
    }
    if ((!(reset)) && ((rising_end->x != falling_start->x) || (rising_end->y != falling_start->y))) 
        reset_falling_end(falling_start, falling_end, rising_start, rising_end);
}
void reset_falling_end(Point* falling_start, Point* falling_end, Point* rising_start, Point* rising_end) {
    int top = 1, id = GET_ID(input_data[rising_start->y][rising_start->x]), direction = GET_DIRECTION(input_data[rising_end->y][rising_end->x + top]); 
    while ((GET_ID(input_data[rising_end->y][rising_end->x + top]) != id) || ((direction <= E) || (direction >= W))) { 
        top++; 
        direction = GET_DIRECTION(input_data[rising_end->y][rising_end->x + top]); 
    }
    Point temp;
    temp.x = rising_end->x + top;
    temp.y = rising_end->y;
    int start_direction = GET_DIRECTION(input_data[temp.y][temp.x]);
    while((GET_DIRECTION(input_data[temp.y][temp.x]) == start_direction) && ((temp.x != falling_end->x) || (temp.y != falling_end->y))) {
        temp.x += offset[start_direction][1];
        temp.y += offset[start_direction][0];
    }
    if ((falling_end->x == temp.x) && (falling_end->y == temp.y)) {
        falling_end->x = rising_end->x + top;
        falling_end->y = rising_end->y;
    }
    else {
        //reset the rising_end point
        int count = 1, lowest_point = 0;
        while (GET_ID(input_data[temp.y][temp.x - count]) != GET_ID(input_data[falling_start->y][falling_start->x]))
            count++;
        rising_end->x = temp.x - count; 
        rising_end->y = temp.y;
        //reset the falling_end point
        while ((temp.x != falling_start->x) || (temp.y != falling_start->y)) {
            lowest_point = (temp.y > lowest_point) ? temp.y : lowest_point;
            int direction = GET_DIRECTION(input_data[temp.y][temp.x]);
            temp.y += offset[direction][0];
            temp.x += offset[direction][1];
        }
        while (temp.y < lowest_point) {
            int direction = GET_DIRECTION(input_data[temp.y][temp.x]);
            temp.y += offset[direction][0];
            temp.x += offset[direction][1];
        }
        falling_end->x = temp.x;
        falling_end->y = temp.y;
    }
} 
bool check_convex(int x, int y) {
    //initialise direction with the starting point
    int current_x = -1, current_y = -1, org_direction = GET_DIRECTION(input_data[x][y]);
    bool init = true;
    while ((current_x != x) || (current_y != y)) { 
        if (init) {
            init = false;
            current_x = x;
            current_y = y;
        }
        //if this point is one of polygon's vertexes
        int future_direction = GET_DIRECTION(input_data[current_y][current_x]);
        if (org_direction != future_direction) {
            bool ret = check_vertexes(org_direction, future_direction, current_x, current_y);
            //if this particular vertex is fine, then set the orginal direction to the future direction
            if (ret)
                org_direction = future_direction;
            else
                return false;
        }
        current_x += offset[future_direction][1];
        current_y += offset[future_direction][0];
    }
    int direction_from_start = GET_DIRECTION(input_data[x][y]);
    return check_vertexes(org_direction, direction_from_start, x, y);
}
bool check_vertexes(int org_direction, int future_direction, int vertex_x, int vertex_y) {
    int coming_from = (org_direction + 4 > NW) ? (org_direction + 4 - 8) : (org_direction + 4);
    int start_direction = (coming_from + 1 > 7) ? N : (coming_from + 1);
    int nb_direction = (future_direction > start_direction) ? (future_direction - start_direction) : (future_direction + 8 - start_direction);
    for(int i = 0; i < nb_direction; i++) {
        int current_direction = (start_direction + i > 7) ? (start_direction + i - 8) : (start_direction + i);
        int current_y = vertex_y + offset[current_direction][0];
        int current_x = vertex_x + offset[current_direction][1]; 
        while ((current_x >= 0) && (current_x < MAX_WIDTH) && (current_y >= 0) && (current_y < MAX_HEIGHT)) {
            if (GET_ID(input_data[current_y][current_x]) < 0)
               break; 
            if (GET_ID(input_data[current_y][current_x]) == GET_ID(input_data[vertex_y][vertex_x]))
                return false;
            current_y += offset[current_direction][0];
            current_x += offset[current_direction][1]; 
        }
    } 
    return true;
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
                printf("%d ", GET_DIRECTION(input_data[y][x]));
            }
            else 
                break;
        } 
        putchar('\n');
        if (nothing)
            break;
    }
}

