#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main() {

    // calculate number of winning charge times
    u_int64_t time = 49877895;
    u_int64_t record = 356137815021882;

    bool found = false;
    u_int64_t result = 0;
    u_int64_t dist = 0;

    for (int i = 0; i < time; i++) {
        dist = i * (time - i);
        if (dist > record) {
            result++;
            found = true;
        } else if (found) {
            break;
        }

    }

    printf("Total: %llu\n", result);

    return 0;
}
