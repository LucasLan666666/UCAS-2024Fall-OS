#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

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


#define MAX   10000000
#define TIMES 200

int data[MAX];
int idx = 0;


void* thread1(void* arg) {
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
    lock_init();
    pthread_create(&p1, NULL, thread1, NULL);
    pthread_create(&p2, NULL, thread2, NULL);
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    int max_diff = get_max_diff();
    printf("Max diff: %d\n", max_diff);
    return 0;
}
