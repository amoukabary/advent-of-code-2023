// advent of code 2023 day 5

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_MAPS 7


struct mapNode {
    u_int64_t dest_start;
    u_int64_t src_start;
    uint64_t length;
    int64_t cast;  // diff between dest and src

} typedef mapNode;

struct range {
    u_int64_t start;
    u_int64_t end;
} typedef range;

struct map {
    mapNode *nodes;
    range *src_ranges;
    uint64_t num_nodes;

} typedef map;


int inRange(range *r, u_int64_t val) {
    return val >= r->start && val <= r->end;
}

uint64_t getDest(map *m, uint64_t src) {
    for (int i = 0; i < m->num_nodes; i++) {
        if (inRange(&m->src_ranges[i], src)) {
            u_int64_t offset = src - m->nodes[i].src_start;
            return m->nodes[i].dest_start + offset; // Calculate destination based on offset
        }
    }
    return src; // If not in any range, return src
}


mapNode *parseMapNode(char *line) {
    mapNode *node = malloc(sizeof(mapNode));
    char *token = strtok(line, " ");
    node->dest_start = atoll(token);
    token = strtok(NULL, " ");
    node->src_start = atoll(token);
    token = strtok(NULL, " ");
    node->length = atoll(token);

    node->cast = node->src_start - node->dest_start;
    return node;
}

map *createMap() {
    map *m = malloc(sizeof(map));
    m->nodes = malloc(sizeof(mapNode) * 100);
    m->src_ranges = malloc(sizeof(range) * 100);
    m->num_nodes = 0;
    return m;
}

void addNode(map *m, mapNode *node) {
    m->nodes[m->num_nodes] = *node;
    range *r = malloc(sizeof(range));
    r->start = node->src_start;
    r->end = node->src_start + node->length;
    m->src_ranges[m->num_nodes] = *r;

    m->num_nodes++;
}


int main(int argc, char* argv[]) {
    FILE *fp;
    if (argc != 2) {
        printf("Usage: %s <input file>\n", argv[0]);
        return 1;
    }
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("Error opening file %s\n", argv[1]);
        return 1;
    }

    // read seeds
    char buff[1000];
    char *temp;

    temp = fgets(buff, 8, fp);
    temp = fgets(buff, 1000, fp); // string of seeds
    u_int64_t *seeds = malloc(sizeof(u_int64_t) * 20);
    int num_seeds = 0;

    char *token = strtok(buff, " ");
    while (token != NULL) {
        seeds[num_seeds] = atoll(token);
        num_seeds++;
        token = strtok(NULL, " ");
    }

    temp = fgets(buff, 1000, fp); // blank line

    map **maps = malloc(sizeof(map *) * NUM_MAPS);
    int num_maps = 0;

    // read maps
    while (!feof(fp)) {

        maps[num_maps] = createMap(); // create map and add to maps array


        temp = fgets(buff, 1000, fp); // map name
        if (temp == NULL) {
            break;
        }

        // printf("Map: %s", buff);

        temp = fgets(buff, 1000, fp); // first node line

        while (temp != NULL && strcmp(temp, "\n") != 0) {

            // parse map node line
            mapNode *node = parseMapNode(temp);
            addNode(maps[num_maps], node);




            temp = fgets(buff, 1000, fp);
        }
        num_maps++;
    }

    // print maps for debugging
    // for (int i = 0; i < num_maps; i++) {
    //     printf("Map %d\n", i);
    //     for (int j = 0; j < maps[i]->num_nodes; j++) {
    //         printf("Node %d: %llu %llu %llu\n", j, maps[i]->nodes[j].dest_start, maps[i]->nodes[j].src_start, maps[i]->nodes[j].length);
    //     }
    // }

    // print map node for each map for debugging
    // print the range and the cast


    // for (int i = 0; i < 1; i++) {

    //     printf("Map %d\n", i);
    //     for (int j = 0; j < maps[i]->num_nodes; j++) {
    //         printf("Node %d: %llu %llu %llu\n", j, maps[i]->nodes[j].dest_start, maps[i]->nodes[j].src_start, maps[i]->nodes[j].length);
    //         printf("Range: %llu %llu\n", maps[i]->src_ranges[j].start, maps[i]->src_ranges[j].end);
    //         printf("Cast: %lld\n", maps[i]->nodes[j].cast);
    //     }
    // }


    // for each seed, get the dest and pass it to the next map
    u_int64_t src = 0;
    u_int64_t *dests = malloc(sizeof(u_int64_t) * num_seeds);
    for (int i = 0; i < num_seeds; i++) {
        src = seeds[i];
        for (int j = 0; j < num_maps; j++) {
            src = getDest(maps[j], src);
        }
        printf("Seed %d: %llu\n", i, src);
        dests[i] = src;
    }

    // find the min and max dest
    u_int64_t min = dests[0];
    u_int64_t max = dests[0];
    for (int i = 1; i < num_seeds; i++) {
        if (dests[i] < min) {
            min = dests[i];
        }
        if (dests[i] > max) {
            max = dests[i];
        }
    }

    printf("Min: %llu\n", min);
    printf("Max: %llu\n", max);


    return 0;
}
