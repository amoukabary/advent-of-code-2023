#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



int main(int argc, char *argv[]) {
    FILE *fp = fopen(argv[1], "r");
    char line[1000];
    size_t len = 0;
    unsigned int *winners = calloc(1000, sizeof(int));
    // allocate 194 ints to rep the number of copies of each game
    // set all to 1 to start
    u_int64_t *copies = calloc(195, sizeof(u_int64_t));
    for (int i = 0; i < 194; i++) {
        copies[i] = 1;
    }

    int game = 0;

    while (fgets(line, 1000, fp) != NULL) {
        // skip first 9 chars of line
        memmove(line, line + 9, strlen(line));

        char *token = strtok(line, " ");

        while (token != NULL && strcmp(token, "|")) {
            if (strcmp(token, " ") == 0) {
                token = strtok(NULL, " ");
                game++;
                continue;
            }
            int index = atoi(token);
            winners[index] = 1;
            token = strtok(NULL, " ");
        }

        int matches = 0;
        token = strtok(NULL, " ");
        while (token != NULL) {
            if (strcmp(token, " ") == 0) {
                token = strtok(NULL, " ");
                continue;
            }

            int index = atoi(token);
            if (winners[index] == 1) {
                matches++;
            }
            token = strtok(NULL, " ");
        }
        if (matches)  {
            printf("game %d: %d matches\n", game, matches);
            for (int i = 1; i <= matches; i++) {
                copies[game + i] += copies[game];
             }
        }

        memset(winners, 0, 1000 * sizeof(int));
        game++;

    }

    u_int64_t sum = 0;
    for (int i = 0; i < game; i++) {
        sum += copies[i];
    }

    printf("%llu\n", sum);
    return 0;

}

