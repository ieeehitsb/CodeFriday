#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#define NODES 5
#define TIMEOUT_MIN 150
#define TIMEOUT_MAX 300

typedef enum {FOLLOWER, CANDIDATE, LEADER} State;

typedef struct {
    int id;
    State state;
    int currentTerm;
    int votesReceived;
    int timeout;
} Node;

Node cluster[NODES];

// Generate a random election timeout
int random_timeout() {
    return (rand() % (TIMEOUT_MAX - TIMEOUT_MIN + 1)) + TIMEOUT_MIN;
}

// Reset all nodes to follower
void reset_followers(int term) {
    for (int i = 0; i < NODES; i++) {
        cluster[i].state = FOLLOWER;
        cluster[i].votesReceived = 0;
        cluster[i].currentTerm = term;
    }
}

// Simulate election process
int start_election(int candidateId) {
    cluster[candidateId].state = CANDIDATE;
    cluster[candidateId].votesReceived = 1;  // votes for itself

    printf("\nNode %d started election for Term %d\n",
           candidateId, cluster[candidateId].currentTerm);

    for (int i = 0; i < NODES; i++) {
        if (i == candidateId) continue;

        // Simulate random voting (some nodes may not vote)
        int vote = rand() % 2;
        if (vote) {
            cluster[candidateId].votesReceived++;
            printf("Node %d voted for Node %d\n", i, candidateId);
        }
    }

    // Check if majority votes received
    if (cluster[candidateId].votesReceived > NODES / 2) {
        cluster[candidateId].state = LEADER;
        printf("\nLeader elected: Node %d (Term %d)\n",
               candidateId, cluster[candidateId].currentTerm);
        return candidateId;
    }

    printf("Node %d failed to get majority votes.\n", candidateId);
    return -1;
}

// Simulate random timeouts and elections
void simulate_raft() {
    int currentTerm = 1;
    int leaderId = -1;

    reset_followers(currentTerm);

    while (leaderId == -1) {
        // Random node triggers election timeout
        int candidateId = rand() % NODES;
        cluster[candidateId].timeout = random_timeout();
        cluster[candidateId].currentTerm = currentTerm;

        printf("\nElection timeout triggered at Node %d (Timeout = %dms)\n",
               candidateId, cluster[candidateId].timeout);

        leaderId = start_election(candidateId);
        currentTerm++;

        // Short delay between terms
        sleep(1);
    }

    printf("\nConsensus achieved! Leader Node %d governs Term %d.\n",
           leaderId, cluster[leaderId].currentTerm);
}

int main() {
    srand(time(NULL));

    for (int i = 0; i < NODES; i++) {
        cluster[i].id = i;
        cluster[i].state = FOLLOWER;
        cluster[i].currentTerm = 0;
        cluster[i].votesReceived = 0;
    }

    printf("=== RAFT LEADER ELECTION SIMULATION ===\n");
    simulate_raft();
    return 0;
}
