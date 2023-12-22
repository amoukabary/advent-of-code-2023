#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROWS 140
#define COLS 141

int is_symbol(char c) {
    return c == '*';
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

struct pos {
    int x;
    int y;
} typedef pos;

struct number {
    unsigned int val;
    pos position;
} typedef num;

struct gear {
    int x;
    int y;
    unsigned int parts[2];
    int part_count;
} typedef gear;

// Read the input file and return a 2D array
char **read_input(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char **grid = (char **)malloc(ROWS * sizeof(char *));
    for (int i = 0; i < ROWS; i++) {
        grid[i] = (char *)malloc((COLS + 1) * sizeof(char));
        fgets(grid[i], COLS + 1, fp);
        grid[i][COLS] = '\0';
    }

    fclose(fp);
    return grid;
}

// Print the grid
void print_grid(char **grid) {
    for (int i = 0; i < ROWS; i++) {
        printf("%s", grid[i]);
    }
    printf("\n");
}

// Collect numbers from the grid
num **collect_nums(char **grid, int *num_count) {
    num **nums = (num **)malloc(ROWS * COLS * sizeof(num *));
    *num_count = 0;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (is_digit(grid[i][j])) {
                unsigned int val = 0;
                int start = j;
                while (j < COLS && is_digit(grid[i][j])) {
                    val = val * 10 + (grid[i][j] - '0');
                    j++;
                }
                nums[*num_count] = malloc(sizeof(num));
                nums[*num_count]->val = val;
                nums[*num_count]->position.x = i;
                nums[*num_count]->position.y = start;
                (*num_count)++;
            }
        }
    }

    return nums;
}


// Create a grid of gears
gear ***create_gear_grid(char **input) {
    gear ***grid = (gear ***)malloc(ROWS * sizeof(gear **));
    for (int i = 0; i < ROWS; i++) {
        grid[i] = (gear **)malloc(COLS * sizeof(gear *));
        for (int j = 0; j < COLS; j++) {
            grid[i][j] = NULL;
            if (is_symbol(input[i][j])) {
                grid[i][j] = malloc(sizeof(gear));
                grid[i][j]->x = i;
                grid[i][j]->y = j;
                grid[i][j]->part_count = 0;
            }
        }
    }
    return grid;
}

void print_gear_grid(gear ***grid) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (grid[i][j] != NULL && grid[i][j]->part_count == 2) {
                printf("*");
            } else {
                printf("%c", '.');
            }
        }
        printf("\n");
    }
}

// Main function
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./sol <input file>\n");
        return 1;
    }

    char **grid = read_input(argv[1]);
    print_grid(grid);

    int num_count = 0;
    num **nums = collect_nums(grid, &num_count);

    gear ***gear_grid = create_gear_grid(grid);

    // For each number, check if there is an adjacent gear and add it to the gear's parts
    for (int i = 0; i < num_count; i++) {
        int x = nums[i]->position.x;
        int y = nums[i]->position.y;

        // Check adjacent cells for gears
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue; // Skip the number itself

                int nx = x + dx;
                int ny = y + dy;

                // Check boundaries and if the cell contains a gear
                if (nx >= 0 && nx < ROWS && ny >= 0 && ny < COLS && gear_grid[nx][ny] != NULL) {
                    if (gear_grid[nx][ny]->part_count < 2) {
                        gear_grid[nx][ny]->parts[gear_grid[nx][ny]->part_count] = nums[i]->val;
                        gear_grid[nx][ny]->part_count++;
                    } else if (gear_grid[nx][ny]->part_count == 2) {
                        printf("Error: Gear at (%d, %d) has more than 2 parts\n", nx, ny);
                        exit(EXIT_FAILURE);
                    }
                }
            }
        }
    }

    print_gear_grid(gear_grid);
    // Process each gear and calculate the sum of multiplications
    unsigned int total_sum = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (gear_grid[i][j] != NULL && gear_grid[i][j]->part_count == 2) {
                total_sum += gear_grid[i][j]->parts[0] * gear_grid[i][j]->parts[1];
            }
        }
    }

    printf("Total Sum of Multiplications: %u\n", total_sum);

    // Free allocated memory
    for (int i = 0; i < ROWS; i++) {
        free(grid[i]);
        for (int j = 0; j < COLS; j++) {
            if (gear_grid[i][j] != NULL) {
                free(gear_grid[i][j]);
            }
        }
        free(gear_grid[i]);
    }
    free(grid);
    free(gear_grid);
    for (int i = 0; i < num_count; i++) {
        free(nums[i]);
    }
    free(nums);

    return 0;
}
