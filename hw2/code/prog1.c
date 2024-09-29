#include <stdlib.h>

int a = 1;
int b;

int
main(void)
{
    static int c = 2;
    int *p;
    p = (int *)malloc(sizeof(int));
    free(p);
    return 0;
}
