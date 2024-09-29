#include <stdio.h>
#include <pthread.h>

#define ARR_SZ 1000000 /* array size     */
#define N      4       /* num of threads */

int arr[ARR_SZ];

typedef struct {
    int start;
    int end;
} thread_data_t;


/* assign */
void*
fill_array(void* arg)
{
    thread_data_t* data = (thread_data_t*)arg;
    for (int i = data->start; i < data->end; i++)
        arr[i] = i;
    pthread_exit(NULL);
}

int main(void) {
    pthread_t threads[N];
    thread_data_t thread_data[N];
    int chunk_size = ARR_SZ / N;

    /* create threads and execute */
    for (int i = 0; i < N; i++) {
        /* divide array into small pieces */
        thread_data[i].start = i * chunk_size;
        thread_data[i].end = (i == N - 1) ? ARR_SZ : (i + 1) * chunk_size;
        pthread_create(&threads[i], NULL, fill_array, (void*)&thread_data[i]);
    }

    /* wait for threads to end */
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    /* sum up */
    long sum = 0;
    for (int i = 0; i < ARR_SZ; i++) {
        sum += arr[i];
    }

    /* print */
    printf("Sum of array elements: %ld\n", sum);

    return 0;
}