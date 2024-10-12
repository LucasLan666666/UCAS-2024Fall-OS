#define _GNU_SOURCE
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <time.h>


/*
** =======================================================
** PETERSON'S ALGORITHM
** {======================================================
*/

int flag[2];
int turn;

void lock_init() {
    flag[0] = flag[1] = 0;
    turn = 0;
}

void lock_acquire(int self) {
    flag[self] = 1;     /* self is requesting */
    turn = 1 - self;    /* let the other go first */
    __sync_synchronize();
    while (flag[1 - self] == 1 && turn == 1 - self)
        ;   /* spin wait */
}

void lock_release(int self) {
    flag[self] = 0;
}

/* ==================================================== */


#define MAX        10000000
#define TIMES      200
#define TEST_TIMES 1000

int data[MAX];
int idx = 0;

int seperate_time[TEST_TIMES];

void* thread1(void* arg) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

    for (int i = 0; i < MAX; i += TIMES * 2) {
        lock_acquire(0);
        for (int j = i; j < i + TIMES * 2; j += 2) {
            data[idx] = j;
            idx++;
        }
        lock_release(0);
    }
    pthread_exit(NULL);
}

void* thread2(void* arg) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(1, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

    for (int i = 0; i < MAX; i += TIMES * 2) {
        lock_acquire(1);
        for (int j = i; j < i + TIMES * 2; j += 2) {
            data[idx] = j + 1;
            idx++;
        }
        lock_release(1);
    }
    pthread_exit(NULL);
}


int main (void) {
    for (int i = 0; i < TEST_TIMES; i++) {
        struct timespec start, end;
        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start);

        pthread_t p1, p2;
        lock_init();
        pthread_create(&p1, NULL, thread1, NULL);
        pthread_create(&p2, NULL, thread2, NULL);
        pthread_join(p1, NULL);
        pthread_join(p2, NULL);

        clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end);
        seperate_time[i] = (end.tv_sec - start.tv_sec) * 1e9 +
                           (end.tv_nsec - start.tv_nsec);
    }
    int total_time = 0;
    int avg_time;
    for (int i = 0; i < TEST_TIMES; i++)
        total_time += seperate_time[i];
    avg_time = total_time / TEST_TIMES;
    printf("Average time: %d ns\n", avg_time);
    return 0;
}
