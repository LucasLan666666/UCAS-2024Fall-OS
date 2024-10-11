#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdint.h>

#define ARR_SZ 1000000 /* length of the array */
#define N      4       /* number of threads */

long arr[ARR_SZ];
long partial_sums[N];

typedef struct {
    int start;
    int end;
    int thread_id;
} thread_data_t;

static void*
partial_sum(void* arg) {
    thread_data_t* data = (thread_data_t*)arg;
    long sum = 0;
    for (int i = data->start; i < data->end; i++) {
        sum += arr[i];
    }
    partial_sums[data->thread_id] = sum;
    pthread_exit(NULL);
}

static void
measure_sum(long *sum, long *time)
{
    struct timespec start, end;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

    pthread_t threads[N];
    thread_data_t thread_data[N];
    int chunk_size = ARR_SZ / N;

    for (int i = 0; i < N; i++) {
        thread_data[i].start = i * chunk_size;
        thread_data[i].end = (i == N - 1) ? ARR_SZ : (i + 1) * chunk_size;
        thread_data[i].thread_id = i;
        pthread_create(&threads[i], NULL, partial_sum, (void*)&thread_data[i]);
    }

    /* waiting for the threads to end */
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    long local_sum = 0;
    for (int i = 0; i < N; i++) {
        local_sum += partial_sums[i];
    }

    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);

    *sum = local_sum;
    *time = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
}

int
main(void)
{
    /* Initialize the array */
    for (long i = 0; i < ARR_SZ; i++) {
        arr[i] = i + 1;
    }

    long time, sum;

    measure_sum(&sum, &time);
    printf("sum:  %ld  time: %ld ns\n", sum, time);

    return 0;
}
