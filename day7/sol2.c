#include <stdio.h>
#include <stdlib.h>

// Typedef struct declarations
typedef struct hand {
    int cards[5];
    int bid;
} hand;

typedef struct hand_list {
    hand hand;
    struct hand_list *next;
} hand_list;

// Function to get card's integer value from its character representation
int card_value(char card) {
    switch (card) {
        case 'A': return 14;
        case 'K': return 13;
        case 'Q': return 12;
        case 'J': return 11;
        case 'T': return 10;

        default: return card - '0'; // Assumes ASCII character input
    }
}

// Function to determine the value of a hand of cards
int hand_value(int *cards) {
    int counts[15] = {0};
    int jacks = 0;

    // Count each card, noting jacks separately
    for (int i = 0; i < 5; i++) {
        if (cards[i] == 11) { // If the card is a Jack
            jacks++;
        } else {
            counts[cards[i]]++;
        }
    }


    int max = 0, second_max = 0;
    for (int i = 1; i < 15; i++) { // card values are from 1 to 14

        if (counts[i] > max) {
            second_max = max;
            max = counts[i];
        } else if (counts[i] > second_max) {
            second_max = counts[i];
        }
    }

    // Determine hand value based on card counts
    max += jacks;
    if (max > 5) {
        max = 5;
    }
    switch (max) {
        case 5: return 7;
        case 4: return 6;
        case 3: return (second_max == 2) ? 5 : 4;
        case 2: return (second_max == 2) ? 3 : 2;
        default: return 1; // All unique cards
    }

}

// Function to compare two hands
int compare_hands(hand *a, hand *b) {
    int aValue = hand_value(a->cards);
    int bValue = hand_value(b->cards);
    if (aValue != bValue) return aValue - bValue;

    // If hand values are equal, compare individual card values
    int val_a = 0;
    int val_b = 0;
    for (int i = 0; i < 5; i++) {
        val_a = a->cards[i];
        val_b = b->cards[i];

        if (val_a == 11) {
            val_a = 1;
        }
        if (val_b == 11) {
            val_b = 1;
        }
        if (val_a != val_b) return val_a - val_b;
    }

    return 0; // Hands are exactly equal
}

// Function to insert a hand into a sorted linked list
void insert_hand(hand_list **list, hand new_hand) {
    hand_list *node = malloc(sizeof(hand_list));
    if (!node) {
        perror("Failed to allocate memory for a new node");
        exit(EXIT_FAILURE);
    }
    node->hand = new_hand;
    node->next = NULL;

    // Insert into the sorted list
    while (*list && compare_hands(&(*list)->hand, &new_hand) < 0) {
        list = &(*list)->next;
    }
    node->next = *list;
    *list = node;
}

// Main function
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: sol <input file>\n");
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("Failed to open input file");
        return 1;
    }

    hand_list *hands = NULL; // Initialize hands list as empty
    char line[100];
    while (fgets(line, sizeof(line), fp)) {
        // Adjust here to parse the input correctly
        hand new_hand;
        for (int i = 0; i < 5; i++) {
            // Parse each card symbol from the line
            new_hand.cards[i] = card_value(line[i]);
        }
        // Parse the bid from the line (last 3 characters)
        new_hand.bid = atoi(&line[6]); // Convert the string starting at position 6 to integer

        insert_hand(&hands, new_hand);
    }
    fclose(fp); // Close the file when done

    // Print and free the linked list
    int rank = 1;
    u_int64_t total = 0;

    for (hand_list *current = hands; current; current = current->next) {
        printf("%d %d %d %d %d %d\n",
               current->hand.cards[0], current->hand.cards[1],
               current->hand.cards[2], current->hand.cards[3],
               current->hand.cards[4], current->hand.bid);
        // Free the node after use if you're done with the list here
        total += current->hand.bid * rank++;

        printf("current_value: %d\n", hand_value(current->hand.cards));
    }

    // Ideally, you would now free the entire list, node by node
    printf("Total: %llu\n", total);
    return 0;
}

