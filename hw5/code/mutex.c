#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

pthread_mutex_t lock;

#define MAX   10000000
#define TIMES 200

int data[MAX];
int idx = 0;

void* thread1(void* arg)
{
    for (int i = 0; i < MAX; i += TIMES * 2) {
        pthread_mutex_lock(&lock);
        for (int j = i; j < i + TIMES * 2; j += 2) {
            data[idx] = j;
            idx++;
        }
        pthread_mutex_unlock(&lock);
    }
    pthread_exit(NULL);
}

void* thread2(void* arg)
{
    for (int i = 0; i < MAX; i += TIMES * 2) {
        pthread_mutex_lock(&lock);
        for (int j = i; j < i + TIMES * 2; j += 2) {
            data[idx] = j + 1;
            idx++;
        }
        pthread_mutex_unlock(&lock);
    }
    pthread_exit(NULL);
}


int get_max_diff(void)
{
    int diff = 0;
    int max_diff = 0;
    for (int i = 0; i < MAX-1; i++) {
        diff = (data[i] - data[i+1]) > 0 ? (data[i] - data[i+1]) :
                                           (data[i+1] - data[i]) ;
        if (diff > max_diff) max_diff = diff;
    }
    return max_diff;
}

int main (void)
{
    pthread_t p1, p2;
    pthread_mutex_init(&lock, NULL);
    pthread_create(&p1, NULL, thread1, NULL);
    pthread_create(&p2, NULL, thread2, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    pthread_mutex_destroy(&lock);
    int max_diff = get_max_diff();
    printf("Max diff: %d\n", max_diff);
    return 0;
}