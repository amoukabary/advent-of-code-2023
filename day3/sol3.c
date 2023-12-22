#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROWS 141
#define COLS 141 // 140 + 1 for newline

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int extraxt_number(char **grid, int x, int y) {

    // given a grid and a position, extract the number at that position (doesnt need to be beginning of number)

    int number = 0;
    int i = 0;

    // find the beginning of the number
    if (!is_digit(grid[x][y])) {

        return -1;

    }
    while (is_digit(grid[x][y + i])) {
        i--;
    }
    i++;

    // extract the number
    while (is_digit(grid[x][y + i])) {
        number = number * 10 + (grid[x][y + i] - '0');
        i++;
    }

    return number;


}

int *get_adjacent_numbers(char **grid, int x, int y) {
    int *adjacent_numbers = (int *)malloc(8 * sizeof(int));
    int adjacent_count = 0;

    int *num_check = (int *)calloc(1000, sizeof(int));



    // check the 8 adjacent positions
    for (int i = -1; i <= 1; i++) {

        for (int j = -1; j <= 1; j++) {

            if (i == 0 && j == 0) {
                continue;
            }

            int number = extraxt_number(grid, x + i, y + j);
            if (number != -1) {
                if (num_check[number] == 1) {
                    continue;
                }
                num_check[number] = 1;
                adjacent_numbers[adjacent_count] = number;
                adjacent_count++;
            }

        }

    }
    if (adjacent_count == 0) {
        free(adjacent_numbers);
        return NULL;
    } else if (adjacent_count > 2) {
        printf("Found more than 2 adjacent numbers at (%d, %d)\n", x, y);
        exit(EXIT_FAILURE);
    }

    return adjacent_numbers;

}



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

void print_grid(char **grid) {
    for (int i = 0; i < ROWS; i++) {
        printf("%s", grid[i]);
    }
    printf("\n");
}


int main(int argc, char *argv[]) {
    char **grid = read_input("input.txt");
    print_grid(grid);

    int sum = 0;



    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {

            if (grid[i][j] == '*') {    // found gear. check adjacent numbers

                int *adjacent_numbers = get_adjacent_numbers(grid, i, j);
                if (adjacent_numbers == NULL) {
                    continue;
                }

                printf("Found gear with 2 adjacent numbers at (%d, %d): ", i, j);
                for (int k = 0; k < 2; k++) {
                    printf("%d ", adjacent_numbers[k]);
                }

                printf("\n");

                sum += adjacent_numbers[0] * adjacent_numbers[1];

                free(adjacent_numbers);

            }

        }
    }

    printf("Sum: %d\n", sum);

    return 0;
}