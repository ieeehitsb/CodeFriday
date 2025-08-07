#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define MAX_BUCKET_SIZE 5
#define MAX_REQUEST_LEN 20
#define INFLOW_DELAY_MICROSECONDS 200000  // 0.2 seconds
#define OUTFLOW_RATE_PER_SECOND 1         // 1 request per second

char bucket[MAX_BUCKET_SIZE][MAX_REQUEST_LEN]; // Circular buffer for requests
int front = 0, rear = 0, count = 0;

pthread_mutex_t lock;

// Function to get current time as a string
void get_current_time(char* buffer) {
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 10, "%H:%M:%S", timeinfo);
}

// Add a request to the bucket
int enqueue(const char* request_id) {
    pthread_mutex_lock(&lock);

    if (count == MAX_BUCKET_SIZE) {
        char time_buf[10];
        get_current_time(time_buf);
        printf("[%s] Dropped: %s (Bucket Full)\n", time_buf, request_id);
        pthread_mutex_unlock(&lock);
        return 0;
    }

    strncpy(bucket[rear], request_id, MAX_REQUEST_LEN);
    rear = (rear + 1) % MAX_BUCKET_SIZE;
    count++;

    char time_buf[10];
    get_current_time(time_buf);
    printf("[%s] Accepted: %s\n", time_buf, request_id);

    pthread_mutex_unlock(&lock);
    return 1;
}

// Remove and process a request from the bucket
void* process_requests(void* arg) {
    while (1) {
        sleep(1 / OUTFLOW_RATE_PER_SECOND);

        pthread_mutex_lock(&lock);

        if (count > 0) {
            char time_buf[10];
            get_current_time(time_buf);
            printf("[%s] Processed: %s\n", time_buf, bucket[front]);
            front = (front + 1) % MAX_BUCKET_SIZE;
            count--;
        }

        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main() {
    pthread_t processor_thread;
    pthread_mutex_init(&lock, NULL);

    // Start the background thread
    pthread_create(&processor_thread, NULL, process_requests, NULL);

    // Simulate incoming requests
    for (int i = 0; i < 10; i++) {
        char request_id[MAX_REQUEST_LEN];
        snprintf(request_id, MAX_REQUEST_LEN, "req-%d", i);
        enqueue(request_id);
        usleep(INFLOW_DELAY_MICROSECONDS);
    }

    // Let the processor run for a few seconds
    sleep(10);

    pthread_mutex_destroy(&lock);
    return 0;
}
