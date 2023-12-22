#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// define global variables
#define RED_MAX 12
#define GREEN_MAX 13
#define BLUE_MAX 14

void min_count(char *color, int count, int *red, int *green, int *blue) {
    if (strcmp(color, "red") == 0) {
        if (count > *red || *red == -1) {
            *red = count;
        }
    } else if (strcmp(color, "green") == 0) {
        if (count > *green || *green == -1) {
            *green = count;
        }
    } else if (strcmp(color, "blue") == 0) {
        if (count > *blue || *blue == -1) {
            *blue = count;
        }
    } else {
        printf("Invalid color: %s\n", color);
    }
}


// parse line
// Parse line
void parse_line(char *line, int *power) {
    char *round, *color;
    char *round_ptr, *vals_ptr;
    char *count_str;
    int red = -1;
    int green = -1;
    int blue = -1;


    // Remove newline character safely
    line[strcspn(line, "\n")] = '\0';

    // Find the colon and move to the start of the data
    char *ptr = strchr(line, ':');
    if (ptr == NULL) {
        printf("No colon found in line: %s\n", line);
    }
    ptr += 2; // Assuming there is a space after the colon

    // Parse rounds
    for (round = strtok_r(ptr, ";", &round_ptr); round != NULL; round = strtok_r(NULL, ";", &round_ptr)) {
        // Parse each count-color pair
        for (count_str = strtok_r(round, ", ", &vals_ptr); count_str != NULL; count_str = strtok_r(NULL, ", ", &vals_ptr)) {
            color = strtok_r(NULL, ", ", &vals_ptr);
            min_count(color, atoi(count_str), &red, &green, &blue);
        }
    }

    if (red == -1) {
        red = 1;
    }
    if (green == -1) {
        green = 1;
    }
    if (blue == -1) {
        blue = 1;
    }


    *power = red * green * blue;
}

int main(int argc, char **argv) {

    int power;
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
        power = 1;
        // skip past
        // line format: Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
        parse_line(buffer, &power);
        sum += power;

        game++;
    }
    // print sum
    printf("Sum: %d\n", sum);

    // close file

    fclose(fp);

}