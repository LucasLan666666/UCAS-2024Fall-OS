#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define ARRAY_SIZE 5
#define RANDOM_MIN 1
#define RANDOM_MAX 20
#define NUM_ITERATIONS 5

pthread_mutex_t lock;
int data[ARRAY_SIZE];
int sum[NUM_ITERATIONS];
int turn = 1;

void* thread1(void* arg) {
    for (int iter = 0; iter < NUM_ITERATIONS; iter++) {
        while (turn != 1);
        pthread_mutex_lock(&lock);
        for (int i = 0; i < ARRAY_SIZE; i++)
            data[i] = rand() % (RANDOM_MAX - RANDOM_MIN + 1) + RANDOM_MIN;
        turn = 2;
        pthread_mutex_unlock(&lock);
    }
    pthread_exit(NULL);
}

void* thread2(void* arg) {
    for (int iter = 0; iter < NUM_ITERATIONS; iter++) {
        while (turn != 2);
        pthread_mutex_lock(&lock);
        for (int i = 0; i < ARRAY_SIZE; i++)
            sum[iter] += data[i];
        turn = 1;
        pthread_mutex_unlock(&lock);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t t1, t2;
    srand(time(NULL));
    pthread_mutex_init(&lock, NULL);
    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_mutex_destroy(&lock);
    for (int i = 0; i < NUM_ITERATIONS; i++)
        printf("Sum[%d]: %d\n", i, sum[i]);
    return 0;
}
