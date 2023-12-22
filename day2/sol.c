#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define global variables
#define RED_MAX 12
#define GREEN_MAX 13
#define BLUE_MAX 14

int valid_count(char *color, int count) {
    if (strcmp(color, "red") == 0) {
        return count <= RED_MAX;
    } else if (strcmp(color, "green") == 0) {
        return count <= GREEN_MAX;
    } else if (strcmp(color, "blue") == 0) {
        return count <= BLUE_MAX;
    } else {
        printf("Invalid color: %s\n", color);
        return 0;
    }
}


// parse line
// Parse line
int parse_line(char *line) {
    char *round, *color;
    char *round_ptr, *vals_ptr;
    char *count_str;

    // Remove newline character safely
    line[strcspn(line, "\n")] = '\0';

    // Find the colon and move to the start of the data
    char *ptr = strchr(line, ':');
    if (ptr == NULL) {
        printf("No colon found in line: %s\n", line);
        return 0;
    }
    ptr += 2; // Assuming there is a space after the colon

    // Parse rounds
    for (round = strtok_r(ptr, ";", &round_ptr); round != NULL; round = strtok_r(NULL, ";", &round_ptr)) {
        // Parse each count-color pair
        for (count_str = strtok_r(round, ", ", &vals_ptr); count_str != NULL; count_str = strtok_r(NULL, ", ", &vals_ptr)) {
            color = strtok_r(NULL, ", ", &vals_ptr);
            if (color == NULL || !valid_count(color, atoi(count_str))) {
                return 0;
            }
        }
    }
    return 1;
}

int main(int argc, char **argv) {

    if (argc != 2) {
        printf("Usage: %s <input file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // load file
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // parse file.
    char buffer[2000];
    int sum = 0;
    int game = 1;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {

        // skip past
        // line format: Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
        if (parse_line(buffer)) {
            printf("Game valid: %d\n", game);
            sum += game;
        }
        game++;
    }
    // print sum
    printf("Sum: %d\n", sum);

    // close file

    fclose(fp);

}