#include <stdio.h>
#include <string.h>

// Structure representing a simple database node
typedef struct {
    int data;
    char role[10];  // "PRIMARY" or "REPLICA"
} DBNode;

// Function to apply replication (copy data from primary to replica)
void replicate(DBNode *primary, DBNode *replica) {
    replica->data = primary->data;
}

// Function to display state of both nodes
void show_state(DBNode primary, DBNode replica) {
    printf("Primary: %d, Replica: %d\n", primary.data, replica.data);
}

int main() {
    // Create primary and replica nodes
    DBNode primary = {10, "PRIMARY"};
    DBNode replica = {0, "REPLICA"};

    // Initial replication
    replicate(&primary, &replica);
    show_state(primary, replica);

    // Update primary database
    primary.data += 5;

    // Replicate after update
    replicate(&primary, &replica);

    // Show final state
    printf("After update -> Primary: %d, Replica: %d\n", primary.data, replica.data);

    return 0;
}
