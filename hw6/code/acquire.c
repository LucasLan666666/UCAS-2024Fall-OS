#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


pthread_mutex_t lock1, lock2, lock3;

void* thread1(void* arg) {
    pthread_mutex_lock(  &lock1);
    pthread_mutex_lock(  &lock2);
    pthread_mutex_lock(  &lock3);
    pthread_mutex_unlock(&lock3);
    pthread_mutex_unlock(&lock2);
    pthread_mutex_unlock(&lock1);
    pthread_exit(NULL);
}

void* thread2(void* arg) {
    pthread_mutex_lock(  &lock1);
    pthread_mutex_lock(  &lock3);
    pthread_mutex_lock(  &lock2);
    pthread_mutex_unlock(&lock2);
    pthread_mutex_unlock(&lock3);
    pthread_mutex_unlock(&lock1);
    pthread_exit(NULL);
}


int main() {
    pthread_t t1, t2;
    for (int i = 0; i < 10000; i++) {
        pthread_mutex_init(&lock1, NULL);
        pthread_mutex_init(&lock2, NULL);
        pthread_mutex_init(&lock3, NULL);
        pthread_create(&t1, NULL, thread1, NULL);
        pthread_create(&t2, NULL, thread2, NULL);
        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
    }
    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);
    pthread_mutex_destroy(&lock3);
    return 0;
}
