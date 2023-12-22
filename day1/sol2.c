#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> // for intptr_t

void findDigits(const char *line, int *first, int *last) {
    intptr_t firstPos = -1;
    intptr_t lastPos = -1;
    char *nums[] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    *first = -1;
    *last = -1;

    for (int i = 0; i < 10; i++) {
        const char *ptr = line;
        while ((ptr = strstr(ptr, nums[i])) != NULL) {
            intptr_t pos = ptr - line;
            if (*first == -1 || pos < firstPos) {
                *first = i;
                firstPos = pos;
            }
            if (*last == -1 || pos > lastPos) {
                *last = i;
                lastPos = pos;
            }
            ptr++;
        }
    }

    int i = 0;
    while (line[i] != '\0') {
        if (line[i] >= '0' && line[i] <= '9') {
            intptr_t pos = i;
            if (*first == -1 || pos < firstPos) {
                *first = line[i] - '0';
                firstPos = pos;
            }
            if (*last == -1 || pos > lastPos) {
                *last = line[i] - '0';
                lastPos = pos;
            }
        }
        i++;
    }
}

int main() {
    char buffer[1000];
    FILE *fp = fopen("input.txt", "r");
    if (fp == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    int sum = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        int first, last;
        findDigits(buffer, &first, &last);
        if (first != -1 && last != -1) {
            sum += first * 10 + last;
        }
    }

    printf("Sum: %d\n", sum);
    fclose(fp);

    return 0;
}
