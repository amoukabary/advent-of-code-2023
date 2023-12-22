#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main() {
    // load file
    char buffer[1000];
    FILE *fp = fopen("input.txt", "r");

    // create output FILE
    FILE *out = fopen("output.txt", "w");


    // read file
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        // get first digit that appears in buffer and write to output
        char *ptr = buffer;
        while (*ptr != '\0') {
            if (*ptr >= '0' && *ptr <= '9') {
                fprintf(out, "%c", *ptr);
                break;
            }
            ptr++;

        }
        // get last digit that appears in buffer and write to output
        ptr = buffer + strlen(buffer) - 1;
        while (*ptr != '\0') {
            if (*ptr >= '0' && *ptr <= '9') {
                fprintf(out, "%c", *ptr);
                break;
            }
            ptr--;
        }

        // write newline to output

        fprintf(out, "\n");


    }

    // close files
    fclose(fp);
    fclose(out);

    // now read all of output.txt and sum the numbers
    fp = fopen("output.txt", "r");
    int sum = 0;
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        sum += atoi(buffer);
    }

    // print sum
    printf("Sum: %d\n", sum);

    // close file
    fclose(fp);


    return 0;

}