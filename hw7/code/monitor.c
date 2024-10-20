#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_TASKS 8

pthread_mutex_t mutex;
pthread_cond_t cond;
int task[MAX_TASKS][2];
int results[MAX_TASKS];

void* thread_function(void* arg) {
    int thread_id = *(int*)arg;
    int sum;
    pthread_mutex_lock(&mutex);
    // Get task
    task[thread_id][0] = rand() % 100;
    task[thread_id][1] = rand() % 100;
    // Simulate task execution
    usleep((rand() % 10 + 1) * 100000);
    sum = task[thread_id][0] + task[thread_id][1];
    results[thread_id] = sum;
    printf("Thread %d: %2d + %2d = %3d\n", thread_id, task[thread_id][0], task[thread_id][1], sum);
    // Inform main thread that the task is done
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[MAX_TASKS];
    int thread_ids[MAX_TASKS];
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    srand(time(NULL));
    // Create threads
    for (int i = 0; i < MAX_TASKS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }
    // Wait for threads to finish
    for (int i = 0; i < MAX_TASKS; i++) {
        pthread_mutex_lock(&mutex);
        while (results[i] == 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        pthread_mutex_unlock(&mutex);
    }
    // Print final result
    int final_result = 0;
    for (int i = 0; i < MAX_TASKS; i++) {
        final_result += results[i];
    }
    printf("Final result: %d\n", final_result);
    // Clean up
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}
