#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct race {
    int time;
    int record;
} typedef race;

int main() {
    race r1 = {49, 356};
    race r2 = {87, 1378};
    race r3 = {78, 1502};
    race r4 = {95, 1882};

    // calculate number of winning charge times
    u_int64_t result = 1; // Start with 1 for multiplication

    int dist = 0;
    int num_wins = 0;

    // Array of races for easier iteration
    race races[] = {r1, r2, r3, r4};

    // Iterating over each race
    for (int i = 0; i < 4; i++) {
        num_wins = 0; // Reset counter for each race

        for (int t = 0; t < races[i].time; t++) {
            dist = t * (races[i].time - t);
            if (dist > races[i].record) {
                num_wins++;
            }
        }

        result *= num_wins; // Multiply the number of wins to the result
    }

    printf("Total winning charge times: %llu\n", result);

    return 0;
}
