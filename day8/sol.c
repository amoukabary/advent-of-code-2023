#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TABLE_SIZE 30000

struct node {
    char *val;
    char *left;
    char *right;
} typedef node;

/*Takes a 3 Characer string and returns an int*/
int hash(char *str) {

    int total = 0;
    for (int i = 0; i < 3; i++) {
        total += (int) str[i] * pow(31, 2 - i);
    }

    return total % TABLE_SIZE;

}



int main () {

    int array[TABLE_SIZE] = {0};

    FILE *fp = fopen("input.txt", "r");
    char buffer[300];
    char *token;

    node **nodes = malloc(sizeof(node*) * TABLE_SIZE);
    fgets(buffer, sizeof(buffer), fp);
    printf("%s", buffer);
    char *path = malloc(sizeof(char) * strlen(buffer));
    strcpy(path, buffer);

    fgets(buffer, sizeof(buffer), fp);
    printf("%s", buffer);
    int index = 0;

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        token = strtok(buffer, " ");
        index = hash(token);
        // printf("%s: %d\n", token, index);
        nodes[index] = malloc(sizeof(node));
        nodes[index]->val = malloc(sizeof(char) * 4);
        strcpy(nodes[index]->val, token);

        token = strtok(NULL, "(");
        token = strtok(NULL, ",");
        nodes[index]->left = malloc(sizeof(char) * 4);
        strcpy(nodes[index]->left, token);
        // skip the space
        token = strtok(NULL, ")");
        token++;
        nodes[index]->right = malloc(sizeof(char) * 6);
        strcpy(nodes[index]->right, token);

    }

    // print nodes
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (nodes[i] != NULL) {
            printf("%d: %s %s %s\n", i, nodes[i]->val, nodes[i]->left, nodes[i]->right);

        }
    }

    fclose(fp);

    u_int64_t steps = 0;
    int current = 0;
    char curr[4] = "AAA\0";
    while(1) {
        current = steps % strlen(path + 1);
        if (path[current] == 'L') {
            strcpy(curr, nodes[hash(curr)]->left);
        } else if (path[current] == 'R') {
            strcpy(curr, nodes[hash(curr)]->right);
        } else {
            printf("ERROR: %c\n", path[current]);
            break;
        }


        steps++;
        if (strcmp(curr, "ZZZ") == 0) {
            printf("FOUND ZZZ\n");
            printf("%llu\n", steps);

            break;
        }
    }


    return 0;
}