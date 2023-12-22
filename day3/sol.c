// Advent of Code 2023 Day 3 solution

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROWS 141
#define COLS 141 // 140 + 1 for newline


int is_symbol(char c) {
    return c != '.' && c != '\0' && (c < '0' || c > '9');
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
    pos **pos;
    unsigned int pos_count;
} typedef num;

// read the input file and return a 2D array of numbers

char **read_input(char *filename) {
    FILE *fp = fopen(filename, "r");
    char **grid = (char **)malloc(ROWS * sizeof(char *));

    for (int i = 0; i < ROWS; i++) {
        grid[i] = (char *)malloc((COLS + 1) * sizeof(char));
        fgets(grid[i], COLS + 1, fp);
        grid[i][COLS] = '\0';
    }

    fclose(fp);

    return grid;
}

void print_grid(char **grid) {
    for (int i = 0; i < ROWS; i++) {
        printf("%s", grid[i]);
    }
    printf("\n");
}

void print_nums(num **nums, int *num_count) {
    for (int i = 0; i < *num_count; i++) {
        printf("%u: ", nums[i]->val);
        for (int j = 0; j < nums[i]->pos_count; j++) {
            printf("(%d, %d) ", nums[i]->pos[j]->x, nums[i]->pos[j]->y);
        }
        printf("\n");
    }

    printf("%d\n", *num_count);
}

num **collect_nums(char **grid, int *num_count) {

    // iterate through the grid and collect the numbers and their positions
    // in the grid. positions of a number are each spot in the grid that the digits
    // of the number occupy

    num **nums = (num **)malloc(ROWS * COLS * sizeof(num *));
    *num_count = 0;
    char *curr_num = malloc(4 * sizeof(char));
    int curr_num_len = 0;


    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS - 1; j++) {
            curr_num_len = 0;
            while (grid[i][j] >= '0' && grid[i][j] <= '9' && j < COLS - 1) {
                curr_num[curr_num_len] = grid[i][j];
                curr_num_len++;
                j++;
            }

            if (curr_num_len > 0) {
                curr_num[curr_num_len] = '\0';
                nums[*num_count] = malloc(sizeof(num));
                nums[*num_count]->val = atoi(curr_num);
                nums[*num_count]->pos = malloc(curr_num_len * sizeof(pos *));
                for (int k = 0; k < curr_num_len; k++) {
                    nums[*num_count]->pos[k] = malloc(sizeof(pos));
                    nums[*num_count]->pos[k]->x = i;
                    nums[*num_count]->pos[k]->y = j - curr_num_len + k;
                }
                nums[*num_count]->pos_count = curr_num_len;
                (*num_count)++;
            }

            // zero out the current number
            for (int k = 0; k < curr_num_len; k++) {
                curr_num[k] = '\0';
            }

        }
    }

    return nums;

}


int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Usage: ./sol <input file>\n");
        return 1;
    }
    char **grid = read_input(argv[1]);

    print_grid(grid);


    int *num_count = malloc(sizeof(int));
    num **nums = collect_nums(grid, num_count);

    print_nums(nums, num_count);

    unsigned int sum = 0;

    // for each number, check if there is an adjacent symbol in the grid
    // if there is, add the number to the sum

    for (int i = 0; i < *num_count; i++) {
        int found = 0;
        for (int j = 0; j < nums[i]->pos_count; j++) {

            // check all 8 adjacent positions for a symbol (not a number or .)
            int x = nums[i]->pos[j]->x;
            int y = nums[i]->pos[j]->y;

            for (int k = x - 1; k <= x + 1; k++) {
                for (int l = y - 1; l <= y + 1; l++) {
                    if (k >= 0 && k < ROWS - 1 && l >= 0 && l < COLS - 1) {
                        if (is_symbol(grid[k][l])) {
                            found = 1;

                        }
                    }
                }
            }
            if (found) {
                sum += nums[i]->val;
                break;
            }
        }
    }

    // free everything
    for (int i = 0; i < ROWS; i++) {
        free(grid[i]);
    }
    free(grid);

    for (int i = 0; i < *num_count; i++) {
        for (int j = 0; j < nums[i]->pos_count; j++) {
            free(nums[i]->pos[j]);
        }
        free(nums[i]->pos);
        free(nums[i]);
    }
    free(nums);
    free(num_count);


    printf("Sum: %u\n", sum);
    return 0;
}



