#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



int main(int argc, char *argv[]) {
    FILE *fp = fopen(argv[1], "r");
    char line[1000];
    size_t len = 0;
    int *winners = calloc(100, sizeof(int));

    int points = 0;


    while (fgets(line, 1000, fp) != NULL) {
        // skip first 9 chars of line
        memmove(line, line + 9, strlen(line));

        char *token = strtok(line, " ");

        while (strcmp(token, "|") && token != NULL) {
            int index = atoi(token);
            winners[index] = 1;
            printf("%s\n", token);
            token = strtok(NULL, " ");
        }

        int matches = 0;
        while (token != NULL) {
            int index = atoi(token);
            if (winners[index] == 1) {
                matches++;
            }
            token = strtok(NULL, " ");
        }
        if (matches) {
            points = points + pow(2, matches - 1);
        }
        printf("%d\n", matches);
        memset(winners, 0, 100 * sizeof(int));
    }

    printf("%d\n", points);

    return 0;
}

