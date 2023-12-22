// Advent of Code 2023 - Day 5
// Program to process map nodes and calculate destinations based on input seeds.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 100
#define BUFFER_SIZE 1000
#define MAX_SEEDS 10
#define NUM_MAPS 7

typedef struct {
    uint64_t destStart, srcStart, length;
    int64_t offset;  // Difference between destStart and srcStart.
} MapNode;

typedef struct {
    uint64_t start, end;
} range;

typedef struct {
    MapNode *nodes;
    range *srcRanges;
    size_t nodeCount;
} Map;

// Check if a value is within a given range.
int isInRange(const range *range, uint64_t value) {
    return value >= range->start && value <= range->end;
}

// Get the destination for a given source value in a map.
uint64_t getDestination(const Map *map, uint64_t src) {
    for (size_t i = 0; i < map->nodeCount; i++) {
        if (isInRange(&map->srcRanges[i], src)) {;
            return src - map->nodes[i].offset;
        }
    }
    return src;  // Return src if not in any range.
}

MapNode *parseMapNode(const char *line) {
    MapNode *node = malloc(sizeof(MapNode));
    if (!node) return NULL;

    char *end;
    node->destStart = strtoull(line, &end, 10);
    node->srcStart = strtoull(end, &end, 10);
    node->length = strtoull(end, NULL, 10);
    node->offset = (int64_t)(node->srcStart - node->destStart);
    return node;
}

// Create and initialize a new map.
Map *createMap() {
    Map *map = malloc(sizeof(Map));
    if (!map) return NULL;

    map->nodes = malloc(sizeof(MapNode) * MAX_NODES);
    map->srcRanges = malloc(sizeof(range) * MAX_NODES);
    map->nodeCount = 0;
    return map;
}

// Add a node to a map.
void addNode(Map *map, const MapNode *node) {
    if (map->nodeCount >= MAX_NODES) return;

    map->nodes[map->nodeCount] = *node;
    range newRange = {node->srcStart, node->srcStart + node->length - 1};
    map->srcRanges[map->nodeCount] = newRange;
    map->nodeCount++;
}

// Free the memory allocated for a map.
void freeMap(Map *map) {
    if (map) {
        free(map->nodes);
        free(map->srcRanges);
        free(map);
    }
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "Error opening file %s\n", argv[1]);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    range seeds[MAX_SEEDS];
    size_t seedCount = 0;

    // Process seeds.
    fgets(buffer, BUFFER_SIZE, file);
    char *token = strtok(buffer, " ");
    token = strtok(NULL, " ");

    while (token != NULL && seedCount < MAX_SEEDS) {
        seeds[seedCount].start = strtoull(token, NULL, 10);
        token = strtok(NULL, " ");
        seeds[seedCount].end = seeds[seedCount].start + strtoull(token, NULL, 10) - 1;
        token = strtok(NULL, " ");
        seedCount++;
    }

    u_int64_t total = 0;
    for (size_t i = 0; i < seedCount; ++i) {
        total += seeds[i].end - seeds[i].start + 1;
    }

    // Skip a line.
    fgets(buffer, BUFFER_SIZE, file);

    // Process maps.
    Map *maps[NUM_MAPS];
    size_t mapCount = 0;
    while (mapCount < NUM_MAPS && fgets(buffer, BUFFER_SIZE, file)) {
        maps[mapCount] = createMap();
        while (fgets(buffer, BUFFER_SIZE, file) && strcmp(buffer, "\n") != 0) {
            MapNode *node = parseMapNode(buffer);
            addNode(maps[mapCount], node);
            free(node);
        }
        mapCount++;
    }

    // Process each seed through the maps.
    u_int64_t min = UINT64_MAX;
    u_int64_t seed_min = 0;
    u_int64_t processed = 0;

    for (size_t i = 0; i < seedCount; i++) {
        for (uint64_t j = seeds[i].start; j <= seeds[i].end; j++) {
            uint64_t temp = j;
            for (size_t k = 0; k < mapCount; ++k) {
                temp = getDestination(maps[k], temp);
            }
            if (temp < min) {
                min = temp;
                seed_min = j;
            }

            processed++;
            if (processed % 50000000 == 0 || processed == total) {
                // print a progress bar
                printf("\r[");
                int progress = (int)((double)processed / total * 50);
                for (int i = 0; i < progress; i++) {
                    printf("=");
                }
                for (int i = progress; i < 50; i++) {
                    printf(" ");
                }
                printf("] %d%%", (int)((double)processed / total * 100));
                fflush(stdout);

            }
            if (processed == total) {
                printf("\n");
            }
        }
    }

    printf("Min: %llu\n", min);
    printf("Seed: %llu\n", seed_min);

    // Cleanup.
    for (size_t i = 0; i < mapCount; ++i) {
        freeMap(maps[i]);
    }
    fclose(file);
    return 0;
}

