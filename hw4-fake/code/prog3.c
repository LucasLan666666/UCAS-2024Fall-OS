#define _GNU_SOURCE
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdint.h>

#define ARR_SZ 1000000 /* length of the array */
#define TIMES  10000   /* number of measurements */
#define N      16    /* number of threads   */

long arr[ARR_SZ];
long partial_sums[N];

typedef struct {
    int start;
    int end;
    int thread_id;
} thread_data_t;

static void*
partial_sum(void* arg) {
    cpu_set_t cpuset; //CPU 核的位图
    CPU_ZERO(&cpuset); //将位图清零
    CPU_SET(N, &cpuset); //设置位图第N 位为 1，表示与 coreN 绑定。N 从 0 开始计数
    sched_setaffinity(pthread_self(), sizeof(cpuset), &cpuset); //将当前线程和 cpuset 位图中指定的核绑定运行
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
    long total_time = 0;
    long total_sum = 0;
    for (int i = 0; i < TIMES; i++) {
        long time, sum;
        measure_sum(&sum, &time);
        total_time += time;
        total_sum += sum;
    }
    long avg_time = total_time / TIMES;
    long avg_sum = total_sum / TIMES;
    printf("Average sum: %ld, Average time: %ld ns\n", avg_sum, avg_time);
    return 0;
}
