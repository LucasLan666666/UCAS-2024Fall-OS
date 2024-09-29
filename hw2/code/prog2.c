#include <stdio.h>
#include <unistd.h>
#include <wait.h>

#define ARR_LEN 10

int
main(void)
{
    int arr[ARR_LEN];
    int i, t, sum;

    for (i = 0; i < ARR_LEN; i++) {
        arr[i] = i + 1;
    }

    sum = 0;
    if ((t = fork()) == 0) {
        for (i = 0; i < ARR_LEN; i++) {
            sum += arr[i];
        }
        printf("sum is %d.\n", sum);
    } else {
        wait(NULL);
        printf("parent process finishes.\n");
    }

    return 0;
}
