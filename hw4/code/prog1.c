#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdint.h>

#define ARR_SZ 1000000 /* length of the array */

long arr[ARR_SZ];

static void
measure_sum(long *sum, long *time)
{
    struct timespec start, end;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

    long i;
    long local_sum = 0;
    for (i = 1; i <= ARR_SZ; i++) {
        local_sum += i;
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
