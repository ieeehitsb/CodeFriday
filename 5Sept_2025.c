#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define BLOOM_SIZE 1000   // size of bit array
#define HASH_SEED1 17
#define HASH_SEED2 29

// Bit array for Bloom filter
unsigned char bloom[BLOOM_SIZE];

// Simple hash function (djb2 style with a seed)
unsigned int hash(const char *str, int seed) {
    unsigned long hash = seed;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return (unsigned int)(hash % (BLOOM_SIZE * 8));
}

// Set a bit in the Bloom filter
void set_bit(unsigned int pos) {
    bloom[pos / 8] |= (1 << (pos % 8));
}

// Check if a bit is set
int check_bit(unsigned int pos) {
    return (bloom[pos / 8] & (1 << (pos % 8))) != 0;
}

// Insert an item into the Bloom filter
void bloom_insert(const char *item) {
    unsigned int h1 = hash(item, HASH_SEED1);
    unsigned int h2 = hash(item, HASH_SEED2);
    set_bit(h1);
    set_bit(h2);
}

// Check if an item is possibly in the set
int bloom_check(const char *item) {
    unsigned int h1 = hash(item, HASH_SEED1);
    unsigned int h2 = hash(item, HASH_SEED2);
    return check_bit(h1) && check_bit(h2);
}

// Driver code
int main() {
    memset(bloom, 0, sizeof(bloom)); // Initialize filter with 0

    // Insert some items
    bloom_insert("apple");
    bloom_insert("banana");
    bloom_insert("cherry");

    // Check items
    printf("apple  → %s\n", bloom_check("apple") ? "Possibly in set" : "Definitely not in set");
    printf("banana → %s\n", bloom_check("banana") ? "Possibly in set" : "Definitely not in set");
    printf("grape  → %s\n", bloom_check("grape") ? "Possibly in set" : "Definitely not in set");
    printf("mango  → %s\n", bloom_check("mango") ? "Possibly in set" : "Definitely not in set");

    return 0;
}