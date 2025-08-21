#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 10

int processes[MAX_PROCESSES];   // process IDs
int alive[MAX_PROCESSES];       // 1 = alive, 0 = crashed
int n;                          // number of processes
int coordinator = -1;           // current leader

// Start an election from a given process
void startElection(int initiator) {
    printf("\nProcess %d initiates an election...\n", initiator);

    int leader = initiator;
    for (int i = 0; i < n; i++) {
        if (processes[i] > initiator && alive[i]) {
            printf("Process %d → Process %d : Election message sent\n", initiator, processes[i]);
            leader = processes[i];
        }
    }

    if (leader == initiator) {
        printf("Process %d did not find higher alive processes.\n", initiator);
    } else {
        printf("Process %d becomes the new coordinator.\n", leader);
    }

    coordinator = leader;
    announceCoordinator();
}

// Announce the new coordinator
void announceCoordinator() {
    printf("\n[Coordinator Announcement] New Leader: Process %d\n", coordinator);
    for (int i = 0; i < n; i++) {
        if (alive[i] && processes[i] != coordinator) {
            printf("Process %d acknowledges Coordinator %d\n", processes[i], coordinator);
        }
    }
}

// Crash a process
void crashProcess(int id) {
    for (int i = 0; i < n; i++) {
        if (processes[i] == id && alive[i]) {
            alive[i] = 0;
            printf("\nProcess %d has crashed!\n", id);
            if (coordinator == id) {
                printf("Coordinator %d failed! Election required.\n", id);
            }
            return;
        }
    }
    printf("Process %d is already dead or not found.\n", id);
}

// Recover a process
void recoverProcess(int id) {
    for (int i = 0; i < n; i++) {
        if (processes[i] == id) {
            alive[i] = 1;
            printf("\nProcess %d has recovered!\n", id);
            if (id > coordinator) {
                printf("Recovered process %d has higher priority. Starting new election...\n", id);
                startElection(id);
            }
            return;
        }
    }
}

// Print current status
void printStatus() {
    printf("\n--- System Status ---\n");
    for (int i = 0; i < n; i++) {
        printf("Process %d : %s\n", processes[i], alive[i] ? "Alive" : "Dead");
    }
    printf("Current Coordinator: %d\n", coordinator);
}

int main() {
    printf("Enter number of processes (max %d): ", MAX_PROCESSES);
    scanf("%d", &n);

    printf("Enter process IDs (unique integers, e.g., 1 2 3 ...):\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &processes[i]);
        alive[i] = 1; // all alive initially
    }

    // Initial coordinator = highest ID
    coordinator = processes[0];
    for (int i = 1; i < n; i++) {
        if (processes[i] > coordinator) {
            coordinator = processes[i];
        }
    }

    printf("\nInitial Coordinator: Process %d\n", coordinator);

    int choice, id;
    while (1) {
        printf("\nMenu:\n");
        printf("1. Crash a process\n");
        printf("2. Recover a process\n");
        printf("3. Start election manually\n");
        printf("4. Print status\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter process ID to crash: ");
                scanf("%d", &id);
                crashProcess(id);
                break;

            case 2:
                printf("Enter process ID to recover: ");
                scanf("%d", &id);
                recoverProcess(id);
                break;

            case 3:
                printf("Enter process ID to start election: ");
                scanf("%d", &id);
                startElection(id);
                break;

            case 4:
                printStatus();
                break;

            case 5:
                exit(0);

            default:
                printf("Invalid choice. Try again.\n");
        }
    }
    return 0;
}